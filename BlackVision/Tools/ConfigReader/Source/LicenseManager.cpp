#include "LicenseManager.h"
#include "Rockey2.h"
#include <string>
#include <stdio.h>

namespace bv{

	bool LicenseManager::Licensed = false;

	LicenseManager::LicenseManager(void)
	{

	}


	LicenseManager::~LicenseManager(void)
	{
	}

	bool LicenseManager::LoadLicenses()
	{
            
            int retcode = RY2_Find();
			if (retcode < 0)
			{
				printf("ERROR: %08x\n", retcode);
				return false;
			}
			if (retcode == 0)
			{
				printf("ERROR: Brak licencji\n");
				return false;
			}

			
			DWORD uid=670789431;
			
			DWORD hid=0;
			retcode = RY2_Open(1, uid, &hid);
            int handle = retcode;
			if (retcode < 0)
			{
				printf("ERROR: %08x\n", retcode);
                RY2_Close(handle);
				return false;
			}
			
			
			
            int block_index=0;
			
            char buffer[512]={0};
			retcode = RY2_Read(handle, block_index, buffer);
			if (retcode < 0)
			{
				printf("ERROR: %08x\n", retcode);
                RY2_Close(handle);
				return false;
			}
			printf("Read data: %s\n", buffer);

            std::string buff = std::string(buffer);
            std::string secret = "tajnehaslo2";

            if(buff == secret)
            {
                printf("License files match.\n");
                Licensed = true;
            }else{
                printf("Error: Wrong license");
                RY2_Close(handle);
                return false;
            }




			RY2_Close(handle);

		
		return true;
	}
	bool LicenseManager::VerifyLicense()
	{
		// yet to be codeds
		return Licensed;
	}

}