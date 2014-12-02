// Yanked from
// https://gist.github.com/wedesoft/3216298

#import <Foundation/Foundation.h>
#import <Foundation/NSUrl.h>
#import <Foundation/NSString.h>

// WARNING: I have no idea if this leaks memory or not. I never wanted to fucking write Obj-C code.

namespace Platform {

    const char* fileIdToPath(const char*fileId) {
        const char* cstr;
        NSString * url = @"";
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
        url = [[[[NSURL URLWithString: [NSString stringWithUTF8String: fileId]] filePathURL] absoluteString] copy];
        cstr = [url UTF8String];
        [pool release];
        return cstr;
    }

}
