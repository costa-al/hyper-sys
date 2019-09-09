
/*----------------------------------------------------------------------------
	HVar.
----------------------------------------------------------------------------*/

class HVar {
protected:
	union {
		INT		m_IntVal;
		FLOAT	m_FloatVal;
		FLOAT	m_VecVal[4];
		FString	*m_StrVal;
		void	*m_PtrVal;
	};

	INT m_nType;

public:
	enum EVarTypes {
		Void    = 0,
		Int     = 'i',
		Float   = 'f',
		Pointer	= 'p',
		String  = 's',
		Vec2	= 'v2',
		Vec3	= 'v',
		Quat	= 'q',
	};

	INT				GetType()		{ return m_nType;		}

	INT				GetInt()		{ return m_IntVal;		}
	FLOAT			GetFloat()		{ return m_FloatVal;	}
	FString*		GetString()		{ return m_StrVal;		}
	HClass*			GetPointer()	{ return (HClass*)m_PtrVal;	}
	HVec2			GetVec2()		{ return HVec2(m_VecVal[0],
														m_VecVal[1]);
									}
	HVec3			GetVec3()		{ return HVec3(m_VecVal[0],
														m_VecVal[1],
														m_VecVal[2]);
									}
	HQuat			GetQuat()		{ return HQuat(m_VecVal[0],
															m_VecVal[1],
															m_VecVal[2],
															m_VecVal[3]);
									}

	HVar()
		:	m_IntVal(0)
		,	m_FloatVal(0.0f)
		,	m_StrVal(NULL)
		,	m_PtrVal(NULL)
	{}
	~HVar() {}
	HVar(const INT Value) {
		m_nType = Int;
		m_IntVal = Value;
	}
	operator INT() const  {
		switch (m_nType) {
			case Int:		return m_IntVal;
			case Float:		return (INT)m_FloatVal;
			case Pointer:	throw("HVar: can't convert Pointer to type INT\n");
			case String:	atoi((*(*m_StrVal)));
			case Vec2:		throw("HVar: can't convert Vec2 to type INT\n");
			case Vec3:		throw("HVar: can't convert Vec3 to type INT\n");
			case Quat:		throw("HVar: can't convert Quat to type INT\n");
			default:		throw("HVar::operator INT(): unknown type of HVar\n");
		}
	}
	void Set(const INT Value) {
		m_nType = Int;
		m_IntVal = Value;
	}
	HVar(const FLOAT Value) {
		m_nType = Float;
		m_FloatVal = Value;
	}
	operator FLOAT() const {
		switch (m_nType) {
			case Int:		return (FLOAT)m_IntVal;
			case Float:		return m_FloatVal;
			case Pointer:	throw("HVar: can't convert Pointer to type FLOAT\n");
			case String:	atof((*(*m_StrVal)));
			case Vec2:		throw("HVar: can't convert Vec2 to type FLOAT\n");
			case Vec3:		throw("HVar: can't convert Vec3 to type FLOAT\n");
			case Quat:		throw("HVar: can't convert Quat to type FLOAT\n");
			default:		throw("HVar::operator FLOAT(): unknown type of HVar\n");
		}
	}
	void Set(const FLOAT Value) {
		m_nType = Float;
		m_FloatVal = Value;
	}
	HVar(FString Value) {
		m_nType = String;
		m_StrVal = new FString(Value);
	}
	operator FString() const {
		TCHAR Str[256];
		switch (m_nType) {
			case Int:		sprintf(&Str[0], "%i", m_IntVal); return FString(Str);
			case Float:		sprintf(&Str[0], "%g", m_FloatVal); return FString(Str);
			case Pointer:	throw("HVar: can't convert Pointer to type String\n");
			case String:	return (*m_StrVal);
			case Vec2:		sprintf(&Str[0], "{%g,%g}", m_VecVal[0], m_VecVal[1]); return FString(Str);
			case Vec3:		sprintf(&Str[0], "{%g,%g,%g}", m_VecVal[0], m_VecVal[1], m_VecVal[2]); return FString(Str);
			case Quat:		sprintf(&Str[0], "{%g,%g,%g,%g}", m_VecVal[0], m_VecVal[1], m_VecVal[2], m_VecVal[3]); return FString(Str);
			default:		throw("HVar::operator FString(): unknown type of HVar\n");
		}
	}
	HVar(const TCHAR* Value) {
		m_nType = String;
		m_StrVal = new FString(Value);
	}
	operator const TCHAR* () const {
		TCHAR *Str = new TCHAR[32];
		switch (m_nType) {
			case Int:		sprintf(Str, "%i", m_IntVal); return Str;
			case Float:		sprintf(Str, "%g", m_FloatVal); return Str;
			case Pointer:	throw("HVar: can't convert Pointer to type String\n");
			case String:	return (*(*m_StrVal));
			case Vec2:		sprintf(Str, "{%g,%g}", m_VecVal[0], m_VecVal[1]); return Str;
			case Vec3:		sprintf(Str, "{%g,%g,%g}", m_VecVal[0], m_VecVal[1], m_VecVal[2]); return Str;
			case Quat:		sprintf(Str, "{%g,%g,%g,%g}", m_VecVal[0], m_VecVal[1], m_VecVal[2], m_VecVal[3]); return Str;
			default:		throw("HVar::operator const TCHAR*(): unknown type of HVar\n");
		}
	}
	void Set(const TCHAR* Value) {
		m_nType = String;
		m_StrVal = new FString(Value);
	}
	void Set(FString* Value) {
		m_nType = String;
		m_StrVal = Value;
	}
	HVar(HClass* Value) {
		m_nType = Pointer;
		m_PtrVal = Value;
	}
	operator HClass*() const {
		switch (m_nType) {
			case Int:		throw("HVar: can't convert INT to type Pointer\n");
			case Float:		throw("HVar: can't convert FLOAT to type Pointer\n");
			case String:	throw("HVar: can't convert String to type Pointer\n");
			case Pointer:	return (HClass*)m_PtrVal;
			case Vec2:		throw("HVar: can't convert Vec2 to type Pointer\n");
			case Vec3:		throw("HVar: can't convert Vec3 to type Pointer\n");
			case Quat:		throw("HVar: can't convert Quat to type Pointer\n");
			default:		throw("HVar::operator HClass*(): unknown type of HVar\n");
		}
	}
	operator void*() const {
		return m_PtrVal;
	}
	HVar(HVec2 Value) {
		m_nType = Vec2;
		m_VecVal[0] = Value[0];
		m_VecVal[1] = Value[1];
	}
	operator HVec2() const {
		switch (m_nType) {
			case Int:		throw("HVar: can't convert INT to type Vec2\n");
			case Float:		throw("HVar: can't convert FLOAT to type Vec2\n");
			case Pointer:	throw("HVar: can't convert Pointer to type Vec2\n");
			case String:	throw("HVar: can't convert String to type Vec2\n");
			case Vec2:		return HVec2(m_VecVal[0], m_VecVal[1]);
			case Vec3:		throw("HVar: can't convert Vec3 to type Vec2\n");
			case Quat:		throw("HVar: can't convert Quat to type Vec2\n");
			default:		throw("HVar::operator HVec2(): unknown type of HVar\n");
		}
	}
	HVar(HVec3 Value) {
		m_nType = Vec3;
		m_VecVal[0] = Value[0];
		m_VecVal[1] = Value[1];
		m_VecVal[2] = Value[2];
	}
	operator HVec3() const {
		switch (m_nType) {
			case Int:		throw("HVar: can't convert INT to type Vec3\n");
			case Float:		throw("HVar: can't convert FLOAT to type Vec3\n");
			case Pointer:	throw("HVar: can't convert Pointer to type Vec3\n");
			case String:	throw("HVar: can't convert String to type Vec3\n");
			case Vec2:		throw("HVar: can't convert Vec2 to type Vec3\n"); 
			case Vec3:		return HVec3(m_VecVal[0], m_VecVal[1], m_VecVal[2]);
			case Quat:		throw("HVar: can't convert Quat to type Vec3\n");
			default:		throw("HVar::operator HVec3(): unknown type of HVar\n");
		}
	}
	HVar(HQuat Value) {
		m_nType = Quat;
		m_VecVal[0] = Value[0];
		m_VecVal[1] = Value[1];
		m_VecVal[2] = Value[2];
		m_VecVal[3] = Value[3];
	}
	operator HQuat() const {
		switch (m_nType) {
			case Int:		throw("HVar: can't convert INT to type Vec3\n");
			case Float:		throw("HVar: can't convert FLOAT to type Vec3\n");
			case Pointer:	throw("HVar: can't convert Pointer to type Vec3\n");
			case String:	throw("HVar: can't convert String to type Vec3\n");
			case Vec2:		throw("HVar: can't convert Quat to type Vec2\n");
			case Vec3:		throw("HVar: can't convert Quat to type Vec3\n");
			case Quat:		return HQuat(m_VecVal[0], m_VecVal[1], m_VecVal[2], m_VecVal[3]);
			default:		throw("HVar::operator HQuat(): unknown type of HVar\n");
		}
	}
};

template <class T> HTBuffer<T>& operator << ( HTBuffer<T>& Ar, HVar& V ) {
	guard(HTBuffer<T>::operator << (HTBuffer<T>& Ar, HVar& V));

	INT				ValInt;
	FLOAT			ValFloat;
	FString*		ValStr;
	HVec2		ValVec2;
	HVec3		ValVec3;
	HQuat	ValQuat;

	if((Ar.m_eRWMode == BUF_READ) && (V.GetType() == String)) {
		ValStr = V.GetString();
		delete ValStr;
	}

	INT nType = V.GetType();
	Ar << nType;
	switch (nType) {
	case Int:
		ValInt = V.GetInt();
		Ar << ValInt;
		break;
	case Float:
		ValFloat = V.GetFloat();
		Ar << ValFloat;
		break;
	case String:
		ValStr = V.GetString();
		Ar << (*(*ValStr));
		break;
	case Vec2:
		ValVec2 = V.GetVec2();
		Ar << (*ValVec2)[0];
		break;
	case Vec3:
		ValVec3 = V.GetVec3();
		Ar << (*ValVec3)[0];
		break;
	case Quat:
		ValQuat = V.GetQuat();
		Ar << (*ValQuat)[0];
		break;
	};

	return Ar;
	unguard;
}

template <> struct HTTypeInfo<HVar> : public FTTypeInfoBase<HVar>
{
public:
	static HBOOL NeedsDestructor() {return 0;}
};

template <class T> inline T CXVarToType(HVar &Var) {
	return (T)Var;
};
template <INT> inline INT CXVarToType<INT>(HVar &Var) {
	return (INT)Var;
};
template <FLOAT> inline FLOAT CXVarToType<FLOAT>(HVar &Var) {
	return (FLOAT)Var;
};
template <DOUBLE> inline DOUBLE CXVarToType<DOUBLE>(HVar &Var) {
	return (DOUBLE)Var;
};
template <const TCHAR*> inline const TCHAR* CXVarToType<const TCHAR*>(HVar &Var) {
	return (const TCHAR*)Var;
};
template <FString> inline FString CXVarToType<FString>(HVar &Var) {
	return (FString)Var;
};
template <FString*> inline FString* CXVarToType<FString*>(HVar &Var) {
	return (FString*)Var;
};
template <HVec2> inline HVec2 CXVarToType<HVec2>(HVar &Var) {
	return (HVec2)Var;
};
template <HVec3> inline HVec3 CXVarToType<HVec3>(HVar &Var) {
	return (HVec3)Var;
};
template <HQuat> inline HQuat CXVarToType<HQuat>(HVar &Var) {
	return (HQuat)Var;
};


/* -------------------------- End -------------------------- */
