#ifndef _INCLUDED_aimmsifc_iAimmsCharTypes
#define _INCLUDED_aimmsifc_iAimmsCharTypes

namespace aimmsifc {

	template <typename char_t>
	struct AimmsStringType{
		int                     Length;
		char_t*                 buf;
	};

	template <typename char_t>
	union AimmsValueType{
		double                  Double;
		int                     Int;
		AimmsStringType<char_t> String;
	};

} // end namespace

#endif // _INCLUDED_aimmsifc_iAimmsCharTypes

