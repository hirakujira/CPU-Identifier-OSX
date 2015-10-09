/* ----------------------------------------------------------------------------
 *   MobileDevice.h - interface to MobileDevice.framework 
 * ------------------------------------------------------------------------- */
#pragma once
#define TARGET_OS_WIN32 0
#define __ITUNES_VER 820

#ifndef MOBILEDEVICE_H
#define MOBILEDEVICE_H

#ifdef __cplusplus
extern "C" {
#endif
	
#ifndef __GCC__
#pragma pack(1)
#define __PACK
#else
	#define __PACK __attribute__((__packed__))
#endif

#if defined(WIN32)
#define __DLLIMPORT
#include <CoreFoundation.h>
typedef unsigned int mach_error_t;
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <mach/error.h>
#endif	
	
	/* Error codes */
#define MDERR_APPLE_MOBILE  (err_system(0x3a))
#define MDERR_IPHONE        (err_sub(0))
	
	/* Apple Mobile (AM*) errors */
#define MDERR_OK                ERR_SUCCESS
#define MDERR_SYSCALL           (ERR_MOBILE_DEVICE | 0x01)
#define MDERR_OUT_OF_MEMORY     (ERR_MOBILE_DEVICE | 0x03)
#define MDERR_QUERY_FAILED      (ERR_MOBILE_DEVICE | 0x04) 
#define MDERR_INVALID_ARGUMENT  (ERR_MOBILE_DEVICE | 0x0b)
#define MDERR_DICT_NOT_LOADED   (ERR_MOBILE_DEVICE | 0x25)
	
	/* Apple File Connection (AFC*) errors */
#define MDERR_AFC_OUT_OF_MEMORY 0x03
	
	/* USBMux errors */
#define MDERR_USBMUX_ARG_NULL   0x16
#define MDERR_USBMUX_FAILED     0xffffffff
	
	/* Messages passed to device notification callbacks: passed as part of
	 * am_device_notification_callback_info. */
#define ADNCI_MSG_CONNECTED     1
#define ADNCI_MSG_DISCONNECTED  2
#define ADNCI_MSG_UNSUBSCRIBED  3
	
#define AMD_IPHONE_PRODUCT_ID   0x1290
	//#define AMD_IPHONE_SERIAL       ""
	
	/* Services, found in /System/Library/Lockdown/Services.plist */
#define AMSVC_AFC                   CFSTR("com.apple.afc")
#define AMSVC_BACKUP                CFSTR("com.apple.mobilebackup")
#define AMSVC_CRASH_REPORT_COPY     CFSTR("com.apple.crashreportcopy")
#define AMSVC_DEBUG_IMAGE_MOUNT     CFSTR("com.apple.mobile.debug_image_mount")
#define AMSVC_NOTIFICATION_PROXY    CFSTR("com.apple.mobile.notification_proxy")
#define AMSVC_PURPLE_TEST           CFSTR("com.apple.purpletestr")
#define AMSVC_SOFTWARE_UPDATE       CFSTR("com.apple.mobile.software_update")
#define AMSVC_SYNC                  CFSTR("com.apple.mobilesync")
#define AMSVC_SCREENSHOT            CFSTR("com.apple.screenshotr")
#define AMSVC_SYSLOG_RELAY          CFSTR("com.apple.syslog_relay")
#define AMSVC_SYSTEM_PROFILER       CFSTR("com.apple.mobile.system_profiler")
	
	typedef unsigned int afc_error_t;
	typedef unsigned int usbmux_error_t;
	
	struct am_recovery_device;
	
	struct am_device_notification_callback_info {
		struct am_device *dev;  /* 0    device */ 
		unsigned int msg;       /* 4    one of ADNCI_MSG_* */
                struct am_device_notification* subscription; 
	} __PACK;
	
	/* The type of the device restore notification callback functions.
	 * TODO: change to correct type. */
	typedef void (*am_restore_device_notification_callback)(struct am_recovery_device *);
	
	/* This is a CoreFoundation object of class AMRecoveryModeDevice. */
	struct am_recovery_device {
		unsigned char unknown0[8];                          /* 0 */
		am_restore_device_notification_callback callback;   /* 8 */
		void *user_info;                                    /* 12 */
		unsigned char unknown1[12];                         /* 16 */
		unsigned int readwrite_pipe;                        /* 28 */
		unsigned char read_pipe;                            /* 32 */
		unsigned char write_ctrl_pipe;                      /* 33 */
		unsigned char read_unknown_pipe;                    /* 34 */
		unsigned char write_file_pipe;                      /* 35 */
		unsigned char write_input_pipe;                     /* 36 */
	} __PACK;
	
	/* A CoreFoundation object of class AMRestoreModeDevice. */
	struct am_restore_device {
		unsigned char unknown[32];
		int port;
	} __PACK;
	
	/* The type of the device notification callback function. */
	typedef void(*am_device_notification_callback)(struct am_device_notification_callback_info *, int cookie);
	
	/* The type of the _AMDDeviceAttached function.
	 * TODO: change to correct type. */
	typedef void *amd_device_attached_callback;
	
	/* The type of the device restore notification callback functions.
	 * TODO: change to correct type. */
	//typedef void (*am_restore_device_notification_callback)(struct am_recovery_device *);

	/* Structure that contains internal data used by AMDevice... functions. Never try 
         * to access its members directly! Use AMDeviceCopyDeviceIdentifier, 
         * AMDeviceGetConnectionID, AMDeviceRetain, AMDeviceRelease instead. */
	struct am_device {
		unsigned char unknown0[16]; /* 0 - zero */
		unsigned int device_id;     /* 16 */
		unsigned int product_id;    /* 20 - set to AMD_IPHONE_PRODUCT_ID */
		char *serial;               /* 24 - set to UDID, Unique Device Identifier */
		unsigned int unknown1;      /* 28 */
		unsigned int unknown2;      /* 32 - reference counter, increased by AMDeviceRetain, decreased by AMDeviceRelease*/
		unsigned int lockdown_conn; /* 36 */
		unsigned char unknown3[8];  /* 40 */
#if (__ITUNES_VER > 740)
		unsigned int unknown4;      /* 48 - used to store CriticalSection Handle*/
#endif
#if (__ITUNES_VER >= 800)
		unsigned char unknown5[24];  /* 52 */
#endif
	} __PACK;
	
	struct am_device_notification {
		unsigned int unknown0;                      /* 0 */
		unsigned int unknown1;                      /* 4 */
		unsigned int unknown2;                      /* 8 */
		am_device_notification_callback callback;   /* 12 */ 
		unsigned int cookie;                      /* 16 */
	} __PACK;
	
	struct afc_connection {
		unsigned int handle;            /* 0 */
		unsigned int unknown0;          /* 4 */
		unsigned char unknown1;         /* 8 */
		unsigned char padding[3];       /* 9 */
		unsigned int unknown2;          /* 12 */
		unsigned int unknown3;          /* 16 */
		unsigned int unknown4;          /* 20 */
		unsigned int fs_block_size;     /* 24 */
		unsigned int sock_block_size;   /* 28: always 0x3c */
		unsigned int io_timeout;        /* 32: from AFCConnectionOpen, usu. 0 */
		void *afc_lock;                 /* 36 */
		unsigned int context;           /* 40 */
	} __PACK;
	
	

	struct afc_device_info {
		unsigned char unknown[12];  /* 0 */
	} __PACK;

	struct afc_directory {
		unsigned char *unknown;   /* size unknown */
	} __PACK;

	struct afc_dictionary {
		unsigned char *unknown;   /* size unknown */
	} __PACK;
	
	typedef unsigned long long afc_file_ref;
	
	struct usbmux_listener_1 {                  /* offset   value in iTunes */
		unsigned int unknown0;                  /* 0        1 */
		unsigned char *unknown1;                /* 4        ptr, maybe device? */
		amd_device_attached_callback callback;  /* 8        _AMDDeviceAttached */
		unsigned int unknown3;                  /* 12 */
		unsigned int unknown4;                  /* 16 */
		unsigned int unknown5;                  /* 20 */
	} __PACK;
	
	struct usbmux_listener_2 {
		unsigned char unknown0[4144];
	} __PACK;
	
	struct am_bootloader_control_packet {
		unsigned char opcode;       /* 0 */
		unsigned char length;       /* 1 */
		unsigned char magic[2];     /* 2: 0x34, 0x12 */
		unsigned char *payload;   /* 4 */
	} __PACK;
	
	/* ----------------------------------------------------------------------------
	 *   Public routines
	 * ------------------------------------------------------------------------- */
	
	/* Starts a service and returns a socket file descriptor that can be used in order to further
	 * access the service. You should stop the session and disconnect before using
	 * the service. iTunes calls this function after starting a session. It starts 
	 * the service and the SSL connection. unknown may safely be
	 * NULL (it is when iTunes calls this), but if it is not, then it will be
	 * filled upon function exit. service_name should be one of the AMSVC_*
	 * constants.
	 *
	 * Returns:
	 *      MDERR_OK                if successful
	 *      MDERR_SYSCALL           if the setsockopt() call failed
	 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
	 */
	 mach_error_t AMDeviceStartService(struct am_device *device, CFStringRef 
									  service_name, int *socket_fd, unsigned int *
									  unknown);

	 /*  Registers a notification with the current run loop. The callback gets
	 *  copied into the notification struct, as well as being registered with the
	 *  current run loop. Cookie gets copied into cookie in the same.
	 *  (Cookie is a user info parameter that gets passed as an arg to
	 *  the callback) unused0 and unused1 are both 0 when iTunes calls this.
	 *
	 *  Never try to acces directly or copy contents of dev and subscription fields 
	 *  in am_device_notification_callback_info. Treat them as abstract handles. 
	 *  When done with connection use AMDeviceRelease to free resources allocated for am_device.
	 *  
	 *  Returns:
	 *      MDERR_OK            if successful
	 *      MDERR_SYSCALL       if CFRunLoopAddSource() failed
	 *      MDERR_OUT_OF_MEMORY if we ran out of memory
	 */
	mach_error_t AMDeviceNotificationSubscribe(am_device_notification_callback callback, 
								unsigned int unused0, unsigned int unused1, 
								unsigned int cookie, 
								struct am_device_notification **subscription);

	/*  Connects to the iPhone. Pass in the am_device structure that the
	 *  notification callback will give to you.
	 *
	 *  Returns:
	 *      MDERR_OK                if successfully connected
	 *      MDERR_SYSCALL           if setsockopt() failed
	 *      MDERR_QUERY_FAILED      if the daemon query failed
	 *      MDERR_INVALID_ARGUMENT  if USBMuxConnectByPort returned 0xffffffff
	 */
	mach_error_t AMDeviceConnect(struct am_device *device);

	/*  Calls PairingRecordPath() on the given device, than tests whether the path
	 *  which that function returns exists. During the initial connect, the path
	 *  returned by that function is '/', and so this returns 1.
	 *
	 *  Returns:
	 *      0   if the path did not exist
	 *      1   if it did
	 */
	
	int AMDeviceIsPaired(struct am_device *device);
	int AMDevicePair(struct am_device *device);
	
	/*  iTunes calls this function immediately after testing whether the device is
	 *  paired. It creates a pairing file and establishes a Lockdown connection.
	 *
	 *  Returns:
	 *      MDERR_OK                if successful
	 *      MDERR_INVALID_ARGUMENT  if the supplied device is null
	 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
	 */
	
	mach_error_t AMDeviceValidatePairing(struct am_device *device);
	
	/*  Creates a Lockdown session and adjusts the device structure appropriately
	 *  to indicate that the session has been started. iTunes calls this function
	 *  after validating pairing.
	 *
	 *  Returns:
	 *      MDERR_OK                if successful
	 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
	 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
	 */
	
	mach_error_t AMDeviceStartSession(struct am_device *device);

	/* Stops a session. You should do this before accessing services.
	 *
	 * Returns:
	 *      MDERR_OK                if successful
	 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
	 */
	
	mach_error_t AMDeviceStopSession(struct am_device *device);
	
	/* Registers for device notifications related to the restore process. unknown0
	 * is zero when iTunes calls this. In iTunes,
	 * the callbacks are located at:
	 *      1: $3ac68e-$3ac6b1, calls $3ac542(unknown1, arg, 0)
	 *      2: $3ac66a-$3ac68d, calls $3ac542(unknown1, 0, arg)
	 *      3: $3ac762-$3ac785, calls $3ac6b2(unknown1, arg, 0)
	 *      4: $3ac73e-$3ac761, calls $3ac6b2(unknown1, 0, arg)
	 */
	unsigned int AMRestoreRegisterForDeviceNotifications(
				am_restore_device_notification_callback dfu_connect_callback,
				am_restore_device_notification_callback recovery_connect_callback,
				am_restore_device_notification_callback dfu_disconnect_callback,
				am_restore_device_notification_callback recovery_disconnect_callback,
				unsigned int unknown0,
				void *user_info);

	/* ----------------------------------------------------------------------------
	 *   Less-documented public routines
	 * ------------------------------------------------------------------------- */
	mach_error_t AMDeviceEnterRecovery(struct am_device *device);
	NSString *AMDeviceCopyValue(struct am_device *device, unsigned int, const NSString *cfstring);

#ifdef __cplusplus
}
#endif

#endif
/* -*- mode:c; indent-tabs-mode:nil; c-basic-offset:2; tab-width:2; */
