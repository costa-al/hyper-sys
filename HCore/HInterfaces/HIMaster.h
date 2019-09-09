
#define IMASTER_SUID FSuid( 0xfd786555, 0x345ea056 )

class FIMaster : public FIUnknown {
public:
	virtual void Process() = 0;
};
