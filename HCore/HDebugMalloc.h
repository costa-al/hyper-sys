// Tags.
enum {MEM_PreTag =0xf0ed1cee};
enum {MEM_PostTag=0xdeadf00f};
enum {MEM_Tag    =0xfe      };
enum {MEM_WipeTag=0xcd      };

// Debug memory allocator.
class FMallocDebug : public FIMalloc
{
private:
	// Structure for memory debugging.
	struct FMemDebugBase
	{
		TCHAR*		Tag;
		SIZE_T		Size;
		INT			RefCount;
		INT			PreTag;
	};
	typedef HTDoubleLinkedList<FMemDebugBase> FMemDebug;

	// Variables.
	FMemDebug* GFirstDebug;
	HBOOL m_bMemInited;

	// Stats.
	INT m_nMallocCalls, m_nReallocCalls, m_nFreeCalls;
	INT m_nMemAllocated, m_nMemMaxAllocated;

public:
	// FMalloc interface.
	FMallocDebug()
	:	GFirstDebug	( NULL )
	,	m_bMemInited	( 0 )
	,	m_nMallocCalls	( 0 )
	,	m_nReallocCalls ( 0 )
	,	m_nFreeCalls	( 0 )
	,	m_nMemAllocated	( 0 )
	,	m_nMemMaxAllocated ( 0 )
	{}
	void* Malloc( DWORD Size, const TCHAR* Tag )
	{
		guard(FMallocDebug::Malloc);
		checkSlow(m_bMemInited);
		check((INT)Size>0);
		FMemDebug* Ptr = NULL;
		guard(CallMalloc);
			Ptr = (FMemDebug*)malloc( sizeof(FMemDebug) + Size + sizeof(INT) );
			check(Ptr);
		unguard;
		TCHAR* DupTag = NULL;
		guard(DupTag);
			DupTag = (TCHAR*)malloc((appStrlen(Tag)+1)*sizeof(TCHAR));
			strcpy( DupTag, Tag );
		unguard;
		guard(SetPtr);
			Ptr->Tag      = DupTag;
			Ptr->RefCount = 1;
			Ptr->Size     = Size;
			Ptr->Next     = GFirstDebug;
			Ptr->PrevLink = &GFirstDebug;
			Ptr->PreTag   = MEM_PreTag;
		unguard;
		guard(SetPost);
			*(INT*)((BYTE*)Ptr+sizeof(FMemDebug)+Size) = MEM_PostTag;
		unguard;
		guard(FillMem);
			memset( Ptr+1, MEM_Tag, Size );
		unguard;
		guard(DoPrevLink);
			if( GFirstDebug )
			{
				check(GIsCriticalError||GFirstDebug->PrevLink==&GFirstDebug);
				GFirstDebug->PrevLink = &Ptr->Next;
			}
			GFirstDebug = Ptr;
		unguard;
		m_nMallocCalls++;
		m_nMemAllocated += Size;
		m_nMemMaxAllocated = (m_nMemMaxAllocated<m_nMemAllocated) ?
			m_nMemAllocated : m_nMemMaxAllocated;
		return Ptr+1;
		unguardf(( TEXT("%i %s"), Size, Tag ));
	}
	void* Realloc( void* InPtr, DWORD NewSize, const TCHAR* Tag )
	{
		guard(FMallocDebug::Realloc);
		checkSlow(m_bMemInited);
		if( InPtr && NewSize )
		{
			check(GIsCriticalError||((FMemDebug*)InPtr-1)->RefCount==1);
			check(GIsCriticalError||((FMemDebug*)InPtr-1)->Size>0);
			void* Result = appMalloc( NewSize, Tag );
			appMemcpy( Result, InPtr, Min(((FMemDebug*)InPtr-1)->Size, NewSize) );
			appFree( InPtr );
			m_nReallocCalls++;
			return Result;
		}
		else if( NewSize )
		{
			m_nReallocCalls++;
			return appMalloc( NewSize, Tag );
		}
		else
		{
			if( InPtr )
				appFree( InPtr );
			m_nReallocCalls++;
			return NULL;
		}
		unguardf(( TEXT("%08X %i %s"), (INT)InPtr, NewSize, Tag ));
	}
	void Free( void* InPtr )
	{
		guard(FMallocDebug::Free);
		checkSlow(m_bMemInited);
		if( !InPtr )
			return;

		FMemDebug* Ptr = (FMemDebug*)InPtr - 1;
		check(GIsCriticalError||Ptr->Size>0);
		check(GIsCriticalError||Ptr->RefCount==1);
		check(GIsCriticalError||Ptr->PreTag==MEM_PreTag);
		check(GIsCriticalError||*(INT*)((BYTE*)InPtr+Ptr->Size)==MEM_PostTag);
		memset( InPtr, MEM_WipeTag, Ptr->Size );
		m_nMemAllocated -= Ptr->Size;
		Ptr->Size = 0;
		Ptr->RefCount = 0;

		check(GIsCriticalError||Ptr->PrevLink);
		check(GIsCriticalError||*Ptr->PrevLink==Ptr);
		*Ptr->PrevLink = Ptr->Next;
		if( Ptr->Next )
			Ptr->Next->PrevLink = Ptr->PrevLink;

		free( Ptr->Tag );
		free( Ptr );

		m_nFreeCalls++;

		unguard;
	}
	void DumpAllocs()
	{
		guard(FMallocDebug::DumpAllocs);
		INT Count=0;
		INT Chunks=0;
		printf("---------------- MEMORY STATISTICS ---------------\n");
		printf( TEXT("Unfreed memory:\n") );
		for( FMemDebug* Ptr=GFirstDebug; Ptr; Ptr=Ptr->Next )
		{
			TCHAR Temp[256];
			strncpy( Temp, (TCHAR*)(Ptr+1), Min((SIZE_T)256, Ptr->Size) );
			printf( TEXT("   % 10i %s <%s>\n"), Ptr->Size, Ptr->Tag, Temp );
			Count += Ptr->Size;
			Chunks++;
		}
		printf( TEXT("\nEnd of list: %i Bytes still allocated"), Count );
		printf( TEXT("\n             %i Chunks allocated\n\n"), Chunks );
		printf( TEXT("Malloc calls:\n\t"
			"MallocCalls     = %5d\n\t"
			"ReallocCalls    = %5d\n\t"
			"FreeCalls       = %5d\n\t"
			"MemAllocated    = %10d / %5d Kb / %3d Mb\n\t"
			"MemMaxAllocated = %10d / %5d Kb / %3d Mb\n\n" ),
				m_nMallocCalls, m_nReallocCalls, m_nFreeCalls,
				m_nMemAllocated, m_nMemAllocated>>10, m_nMemAllocated>>20,
				m_nMemMaxAllocated, m_nMemMaxAllocated>>10, m_nMemMaxAllocated>>20);
		printf("--------------------------------------------------\n");
		unguard;
	}
	void HeapCheck()
	{
		guard(FMallocDebug::HeapCheck);
		for( FMemDebug** Link = &GFirstDebug; *Link; Link=&(*Link)->Next )
			check(GIsCriticalError||*(*Link)->PrevLink==*Link);
		unguard;
	}
	void Init()
	{
		guard(FMallocDebug::Init);
		check(!m_bMemInited);
		m_bMemInited=1;
		unguard;
	}
	void Exit()
	{
		guard(FMallocDebug::Exit);
		check(m_bMemInited);
		m_bMemInited=0;
		unguard;
	}
};

/* -------------------------- End -------------------------- */