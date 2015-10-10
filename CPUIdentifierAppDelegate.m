//
//  CPUIdentifierAppDelegate.m
//  CPUIdentifier
//


#import "CPUIdentifierAppDelegate.h"

static AppDelegate *classPointer;
struct am_device* device;
struct am_device_notification *notification;

void notification_callback(struct am_device_notification_callback_info *info, int cookie) {	
	if (info->msg == ADNCI_MSG_CONNECTED) {
		NSLog(@"Device connected.");
		device = info->dev;
		AMDeviceConnect(device);
		AMDevicePair(device);
		AMDeviceValidatePairing(device);
		AMDeviceStartSession(device);
		[classPointer populateData];
	} else if (info->msg == ADNCI_MSG_DISCONNECTED) {
		NSLog(@"Device disconnected.");
		[classPointer dePopulateData];
	} else {
		NSLog(@"Received device notification: %d", info->msg);
	}
}

void recovery_connect_callback(struct am_recovery_device *rdev) {
	[classPointer recoveryCallback];
}

void recovery_disconnect_callback(struct am_recovery_device *rdev) {
	[classPointer dePopulateData];
}


@interface NSAttributedString (Hyperlink)
+(id)hyperlinkFromString:(NSString*)inString withURL:(NSURL*)aURL alignment:(NSTextAlignment)alignment;
@end

@implementation NSAttributedString (Hyperlink)
+(id)hyperlinkFromString:(NSString*)inString withURL:(NSURL*)aURL alignment:(NSTextAlignment)alignment
{
    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString: inString];
    NSRange range = NSMakeRange(0, [attrString length]);
    
    [attrString beginEditing];
    [attrString addAttribute:NSLinkAttributeName value:[aURL absoluteString] range:range];
    
    // make the text appear in blue
    [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
    [attrString addAttribute:NSFontAttributeName value:[NSFont systemFontOfSize:12] range:range];
    NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setAlignment:alignment];
    [attrString addAttribute:NSParagraphStyleAttributeName value:paragraphStyle range:range];
    
    // next make the text appear with an underline
    [attrString addAttribute:
     NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSSingleUnderlineStyle] range:range];
    
    [attrString endEditing];
    
    return [attrString autorelease];
}
@end

@implementation AppDelegate

@synthesize window, loadingInd;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	classPointer = self;
	AMDeviceNotificationSubscribe(notification_callback, 0, 0, 0, &notification);
	AMRestoreRegisterForDeviceNotifications(recovery_disconnect_callback, recovery_connect_callback, recovery_disconnect_callback, recovery_disconnect_callback, 0, NULL);
    
    accept = NO;
    connected = NO;
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Improve Our Statistics"];
    [alert setInformativeText:@"To represent the distribution of CPU manufactory, we collect some information (device model, CPU model and hashed MAC Address) from your iPhone. We can't identify you nor censor your device with this tiny app. If you want to help us improve the statistics, please click \'OK\'"];

    [alert addButtonWithTitle:@"No, thanks"];
    [alert addButtonWithTitle:@"OK"];
    NSArray *buttons = [alert buttons];
    // note: rightmost button is index 0
    [[buttons objectAtIndex:0] setKeyEquivalent:@"\033"];
    [[buttons objectAtIndex:1] setKeyEquivalent:@"\r"];
    [alert beginSheetModalForWindow:self.window completionHandler:^(NSModalResponse returnCode) {
        if(returnCode == NSAlertSecondButtonReturn)
        {
            accept = YES;
            if (connected == YES) {
                [self populateData];
            }
        }
        else
        {
            accept = NO;
            if (connected == YES) {
                [self populateData];
            }
        }
    }];
    
    [self setHyperlinkWithTextField:twitter string:@"by @hirakujira " url:@"http://twitter.com/hirakujira" alignment:NSTextAlignmentLeft];
    [self setHyperlinkWithTextField:github string:@"GitHub Repo " url:@"http://github.com/hirakujira/CPU-Identifier-OSX" alignment:NSTextAlignmentRight];
    [self setHyperlinkWithTextField:statistics string:@"Show Statistics" url:@"http://demo.hiraku.tw/CPUIdentifier/chart2.php" alignment:NSTextAlignmentRight];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
	return YES;
}


-(void)setHyperlinkWithTextField:(NSTextField*)inTextField string:(NSString *)stringText url:(NSString *)urlString alignment:(NSTextAlignment)alignment
{
    // both are needed, otherwise hyperlink won't accept mousedown
    [inTextField setAllowsEditingTextAttributes: YES];
    [inTextField setSelectable: YES];
    
    NSURL* url = [NSURL URLWithString:urlString];
    
    NSMutableAttributedString* string = [[NSMutableAttributedString alloc] init];
    [string appendAttributedString: [NSAttributedString hyperlinkFromString:stringText withURL:url alignment:alignment]];

    // set the attributed string to the NSTextField
    [inTextField setAttributedStringValue: string];
    
    [string release];
}

- (void)recoveryCallback {
	[disconnetLabel setHidden:YES];
	[loadingInd setHidden:YES];
}

- (void)populateData {
    connected = YES;
    NSString *SN = [[self getDeviceValue:@"SerialNumber"] MD5String];
    NSString *platform = [self getDeviceValue:@"HardwarePlatform"];
	NSString *modelNumber = [self getDeviceValue:@"ModelNumber"];
	NSString *deviceString = [self getDeviceValue:@"ProductType"];
    NSString *region = [self getDeviceValue:@"RegionCode"];
    
    if ([platform isEqualToString:@"s8000"]) {
        platform = @"s8000 (Samsung)";
    }
    else if ([platform isEqualToString:@"s8003"]) {
        platform = @"s8003 (TSMC)";
    }
	
	if ([deviceString isEqualToString:@"iPod1,1"])
		deviceString = @"iPod touch 1Gen";
	else if ([deviceString isEqualToString:@"iPod2,1"])
		deviceString = @"iPod touch 2Gen";
    else if ([deviceString isEqualToString:@"iPod3,1"])
		deviceString = @"iPod touch 3Gen";
    else if ([deviceString isEqualToString:@"iPod4,1"])
		deviceString = @"iPod touch 4Gen";
    else if ([deviceString isEqualToString:@"iPod5,1"])
		deviceString = @"iPod touch 5Gen";
    else if ([deviceString isEqualToString:@"iPod6,1"])
        deviceString = @"iPod touch 6Gen";
    //iPhone
	else if ([deviceString isEqualToString:@"iPhone1,1"])
		deviceString = @"iPhone";
	else if ([deviceString isEqualToString:@"iPhone1,2"])
		deviceString = @"iPhone 3G";
	else if ([deviceString isEqualToString:@"iPhone2,1"])
		deviceString = @"iPhone 3GS";
	else if ([deviceString isEqualToString:@"iPhone3,1"])
		deviceString = @"iPhone 4";
    else if ([deviceString isEqualToString:@"iPhone3,2"])
		deviceString = @"iPhone 4 RevA";
    else if ([deviceString isEqualToString:@"iPhone3,3"])
		deviceString = @"iPhone 4 CDMA";
    else if ([deviceString isEqualToString:@"iPhone4,1"])
		deviceString = @"iPhone 4S";
    else if ([deviceString isEqualToString:@"iPhone5,1"])
		deviceString = @"iPhone 5 GSM";
    else if ([deviceString isEqualToString:@"iPhone5,2"])
		deviceString = @"iPhone 5 Global";
    else if ([deviceString isEqualToString:@"iPhone5,3"])
		deviceString = @"iPhone 5c";
    else if ([deviceString isEqualToString:@"iPhone5,4"])
		deviceString = @"iPhone 5c";
    else if ([deviceString isEqualToString:@"iPhone6,1"])
		deviceString = @"iPhone 5s";
    else if ([deviceString isEqualToString:@"iPhone6,2"])
		deviceString = @"iPhone 5s";
    else if ([deviceString isEqualToString:@"iPhone7,1"])
        deviceString = @"iPhone 6 Plus";
    else if ([deviceString isEqualToString:@"iPhone7,2"])
        deviceString = @"iPhone 6";
    else if ([deviceString isEqualToString:@"iPhone8,1"])
        deviceString = @"iPhone 6s";
    else if ([deviceString isEqualToString:@"iPhone8,2"])
        deviceString = @"iPhone 6s Plus";
    //iPad
	else if ([deviceString isEqualToString:@"iPad1,1"])
		deviceString = @"iPad";
    else if ([deviceString isEqualToString:@"iPad2,1"])
		deviceString = @"iPad 2 Wifi";
    else if ([deviceString isEqualToString:@"iPad2,2"])
		deviceString = @"iPad 2 GSM";
    else if ([deviceString isEqualToString:@"iPad2,3"])
		deviceString = @"iPad 2 CDMA";
    else if ([deviceString isEqualToString:@"iPad2,4"])
		deviceString = @"iPad 2 Wifi RevA";
    else if ([deviceString isEqualToString:@"iPad3,1"])
		deviceString = @"iPad 3 Wifi";
    else if ([deviceString isEqualToString:@"iPad3,2"])
		deviceString = @"iPad 3 Global";
    else if ([deviceString isEqualToString:@"iPad3,3"])
		deviceString = @"iPad 3 GSM";
    else if ([deviceString isEqualToString:@"iPad3,4"])
		deviceString = @"iPad 4 Wifi";
    else if ([deviceString isEqualToString:@"iPad3,5"])
		deviceString = @"iPad 4 GSM";
    else if ([deviceString isEqualToString:@"iPad3,6"])
		deviceString = @"iPad 4 Global";
    else if ([deviceString isEqualToString:@"iPad4,1"])
		deviceString = @"iPad Air Wifi";
    else if ([deviceString isEqualToString:@"iPad4,2"])
		deviceString = @"iPad Air Cellular";
    //iPad mini
    else if ([deviceString isEqualToString:@"iPad2,5"])
		deviceString = @"iPad mini Wifi";
    else if ([deviceString isEqualToString:@"iPad2,6"])
		deviceString = @"iPad mini GSM";
    else if ([deviceString isEqualToString:@"iPad2,7"])
		deviceString = @"iPad mini Global";
    else if ([deviceString isEqualToString:@"iPad4,3"])
		deviceString = @"iPad mini 2 Wifi";
    else if ([deviceString isEqualToString:@"iPad4,4"])
		deviceString = @"iPad mini 2 Cellular";
	else
		deviceString = @"Unknown";
	
	if ([deviceString isEqualToString:@"Unknown"]) {
		NSString *completeString = [NSString stringWithFormat:@"%@ Mode/Device Detected",deviceString];
		[devicetype setStringValue:completeString];
	} else {
		[loadingInd setHidden:YES];
		NSString *completeString = [NSString stringWithFormat:@"%@", deviceString];
		[devicetype setStringValue:completeString];
	}

    [deviceCPU setStringValue:platform];
    [deviceModel setStringValue:[NSString stringWithFormat:@"%@%@",modelNumber,region]];
    [disconnetLabel setHidden:YES];
    

    if (accept == YES) {
        NSURLSessionConfiguration *defaultConfigObject = [NSURLSessionConfiguration defaultSessionConfiguration];
        NSURLSession *defaultSession = [NSURLSession sessionWithConfiguration: defaultConfigObject delegate: self delegateQueue: [NSOperationQueue mainQueue]];
        NSString* urlString = [[NSString alloc] initWithFormat:@"%@?adid=%@&device_type=%@&model=%@&region=%@&chip=%@",@"",SN, [self getDeviceValue:@"ProductType"], modelNumber, region, [self getDeviceValue:@"HardwarePlatform"]];
        NSURL * url = [NSURL URLWithString:urlString];
        
        NSURLSessionDataTask * dataTask = [defaultSession dataTaskWithURL:url
                                                        completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        }];
        
        [dataTask resume];

    }
    
}

- (void)dePopulateData {
    [disconnetLabel setHidden:YES];
    [deviceCPU setStringValue:@""];
    [deviceModel setStringValue:@""];
    [devicetype setStringValue:@""];
}

- (void)loadingProgress {
	[loadingInd setHidden:NO];
	[loadingInd startAnimation: self];
}

- (NSString *)getDeviceValue:(NSString *)value {
	return AMDeviceCopyValue(device, 0, value);
}

@end
