#pragma once
namespace bv{
	class LicenseManager
	{
	private:
		static bool Licensed;
	public:
		LicenseManager(void);
		~LicenseManager(void);
		static bool LoadLicenses();
		static bool VerifyLicense();
	};

}