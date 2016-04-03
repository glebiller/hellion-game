#ifdef __OBJC__

#include <Generic/Framework.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <boost/system/error_code.hpp>

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    Framework* framework_;
    NSTimer* mTimer;
}

- (void)execute:(id)sender;
- (void)shutdown;

@property (retain, atomic) NSTimer* mTimer;

@end

#if __LP64__
static id mAppDelegate;
#endif

@implementation AppDelegate

@synthesize mTimer;

- (void)applicationDidFinishLaunching:(NSNotification *)application {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mTimer = nil;

    framework_ = new Framework();
    if (framework_->Initialize() != boost::system::errc::success) {
        [self shutdown];
        return;
    }

    mTimer = [[NSTimer timerWithTimeInterval: 0.001 target:self selector:@selector(execute:) userInfo:self repeats:true] retain];
    [[NSRunLoop currentRunLoop] addTimer:mTimer forMode: NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:mTimer forMode: NSEventTrackingRunLoopMode];
    [pool release];
}

- (void)execute:(id)sender {
    if (!framework_->Execute()) {
        [NSApp terminate:self];
    }
}

- (void)shutdown {
    [NSApp terminate:nil];
}

@end

int main(int argc, char *argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    mAppDelegate = [[AppDelegate alloc] init];
    [[NSApplication sharedApplication] setDelegate:mAppDelegate];
    int retVal = NSApplicationMain(argc, (const char **) argv);

    [pool release];

    return retVal;
}

#endif