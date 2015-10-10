//
//  CPUIdentifierAppDelegate.h
//  CPUIdentifier
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "MobileDevice.h"
#import "NSString+MD5Addition.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, NSURLSessionDelegate> {
    NSWindow *window;
	IBOutlet NSProgressIndicator *loadingInd;
	IBOutlet NSTextField *disconnetLabel;
    IBOutlet NSTextField *devicetype;
    IBOutlet NSTextField *deviceModel;
    IBOutlet NSTextField *deviceCPU;
    IBOutlet NSTextField *twitter;
    IBOutlet NSTextField *github;
    IBOutlet NSTextField *statistics;
    BOOL accept;
    BOOL connected;
}

- (void)populateData;
- (void)dePopulateData;
- (void)recoveryCallback;
- (void)loadingProgress;
- (NSString *)getDeviceValue:(NSString *)value;
@property (assign) IBOutlet NSWindow *window;
@property (nonatomic, retain) NSProgressIndicator *loadingInd;

@end
