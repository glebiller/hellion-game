#ifdef __OBJC__

#include <Generic/Framework.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

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
    NSDate* mDate;
    NSTimeInterval mLastFrameTime;
}

- (void)execute:(id)sender;
- (void)shutdown;

@property (retain, atomic) NSTimer *mTimer;
@property (nonatomic) NSTimeInterval mLastFrameTime;

@end

#if __LP64__
static id mAppDelegate;
#endif

@implementation AppDelegate

@synthesize mTimer;
@dynamic mLastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)applicationDidFinishLaunching:(NSNotification *)application {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
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
    framework_->Execute();
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