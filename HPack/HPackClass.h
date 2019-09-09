
class HClassTest : public HClass {
public:
	CLASS_PROTOTYPE( HClassTest )

	FUNC_PROTOTYPE( Printf );

	PROP_PROTOTYPE( const TCHAR*,	Name1		)
	PROP_PROTOTYPE( INT,			Testprop1	)

	HClassTest()	{}
	~HClassTest()	{}

};

FUNC_DECL( HClassTest, Printf )
	printf((const TCHAR*)Vars[0]);
END_FUNC

HEventDef1		EV_Printf( "Printf", "s" );

CLASS_DECLARATION( HClass, HClassTest, ECF_Pack )
	EVENT_DECL( EV_Printf,	HClassTest::Printf )
PROPERTIES( HClassTest )
	PROP_DECL( Name1 )	
	PROP_DECL( Testprop1 )
END_CLASS
