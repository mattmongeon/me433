/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "hidapi.h"

// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[256];
	#define MAX_STR 255
	hid_device *handle;

#ifdef WIN32
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif

	struct hid_device_info *devs, *cur_dev;
	
	if (hid_init())
		return -1;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;
	

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(0x04d8, 0x003f, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	char message[15];
	std::cout << "Enter string message: ";
    scanf("%s", message);

	int row = 0;
    std::cout << "Enter row number: ";
	scanf("%d", &row);

	
	// --- Send String To PIC --- //

	memset(buf, 0x20, sizeof(buf));
	buf[0] = 0x1;
	buf[1] = 0x1;
	buf[2] = static_cast<unsigned char>(row);

    for( std::size_t i = 0; i <= strlen(message); ++i )
	{
		buf[i+3] = message[i];
	}

	res = hid_write(handle, buf, 18);
	if (res < 0)
	{
        std::cout << "Unable to write()" << std::endl;
        std::cout << "Error: " << hid_error(handle) << std::endl;
	}

	
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);


	// --- Wait For Accel Data --- //
	
	res = 0;
    int count = 0;
	while(true)
	{
        // --- Request Accel Data --- //

	    memset(buf, 0x20, sizeof(buf));
	    buf[1] = 0x2;
	    res = hid_write(handle, buf, 18);


        // --- Check For Valid Data --- //

		res = hid_read(handle, buf, 18);
        if( res > 0 )
        {
            if( buf[0] != 0 )
            {
                std::cout << "Received acceleration data! - " << count << std::endl;
		        short* accels = reinterpret_cast<short*>(&buf[1]);
		        std::cout << "X accel: " << accels[0] << std::endl;
		        std::cout << "Y accel: " << accels[1] << std::endl;
		        std::cout << "Z accel: " << accels[2] << std::endl;
                std::cout << std::endl;

                ++count;
            }
        }
		else if( res < 0 )
		{
            std::cout << "Unable to read()" << std::endl;
            std::cout << "Error: " << hid_error(handle) << std::endl;
            break;
		}
	}


	// --- Exit And Clean Up --- //

	hid_close(handle);
	hid_exit();

	return 0;
}
