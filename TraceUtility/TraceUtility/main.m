//
//  main.m
//  TraceUtility
//
//  Created by Qusic on 7/9/15.
//  Copyright (c) 2015 Qusic. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

#define NSPrint(format, ...) CFShow((__bridge CFStringRef)[NSString stringWithFormat:format, ## __VA_ARGS__])

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Required. Each instrument is a plugin and we have to load them before we can process their data.
        PFTLoadPlugins();

        // Open a trace document.
        XRTrace *trace = [[XRTrace alloc]initForCommandLine:NO];
        NSError *error;
        NSString *tracePath = [NSString stringWithFormat:@"%s", argv[1]];
        [trace loadDocument:[NSURL fileURLWithPath:tracePath] error:&error];
        if (error) {
            NSLog(@"Error: %@", error);
            return 1;
        }
        
        // Each trace document consists of data from several different instruments.
        for (XRInstrument *instrument in trace.basicInstruments.allInstruments) {

            // You can have multiple runs for each instrument.
            for (XRRun *run in instrument.allRuns) {
                // Here is only one example for runs of the instrument Time Profiler. However it is not difficult for other instruments once we get started.
                if ([run isKindOfClass:NSClassFromString(@"XRCountersRun")]) {
                    
                    XRSamplerRun *sr = (XRSamplerRun *) run;
                    XRStorageManager *sm = sr.storageManager;

                    
                    XRCountersRun *cr = (XRCountersRun *) run;
                    id i = [cr eventsAndFormulas];
                    for (NSDictionary *l in i) {
                        int val = 0;
                        for (int i=0; i<sm.lastEventIdentifier+1; i++) {
                            XRCountersEvent *xc = [sm eventForIdentifier:i];
                            val += *(xc.counterValues+[[l objectForKey:@"eventsIndex"] intValue]);
                        }
                        NSPrint(@"%@",[l objectForKey:@"aliasOrMnemonic"]);
                        NSPrint(@"%d",val);
                    }
                }
                NSPrint(@"\n");
            }

        }
    }
    return 0;
}
