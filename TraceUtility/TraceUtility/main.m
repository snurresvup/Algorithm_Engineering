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
        id test = @"../../../../../../../Applications/Xcode.app/";
        [DVTDeveloperPaths initializeApplicationDirectoryName:test];

        PFTLoadPlugins();

        // Open a trace document.
        XRTrace *trace = [[XRTrace alloc]initForCommandLine:NO];
        NSError *error;
        BOOL verbose = false;
        if (argv[1] == NULL) return -1;
        if (argv[2] != NULL && [[NSString stringWithFormat:@"%s", argv[2]] isEqualToString:@"-v"]) verbose = true;
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
                    static NSMutableArray * (^ const flattenTree)(PFTCallTreeNode *) = ^(PFTCallTreeNode *rootNode) {
                        NSMutableArray *nodes = [NSMutableArray array];
                        if (rootNode) {
                            [nodes addObject:rootNode];
                            for (PFTCallTreeNode *node in rootNode.children) {
                                [nodes addObjectsFromArray:flattenTree(node)];
                            }
                        }
                        return nodes;
                    };
                    
                    XRSamplerRun *sr = (XRSamplerRun *) run;

                    
                    XRBacktraceRepository *backtraceRepository = sr.backtraceRepository;
                    [backtraceRepository refreshTreeRoot]; // Load the tree.
                    
                    NSUInteger sd = -1;
                    
                    // Process the data as you want.
                    NSMutableArray *nodes = flattenTree(backtraceRepository.rootNode);
                    [nodes sortUsingDescriptors:@[[NSSortDescriptor sortDescriptorWithKey:NSStringFromSelector(@selector(terminals)) ascending:NO]]];
                    for (PFTCallTreeNode *node in nodes) {
                        // See the header file for more information about properties of nodes.
                        if (node.symbolNamePath && node.symbolNamePath.count != 0) {
                            if ([node.symbolNamePath[node.symbolNamePath.count-1] isEqualToString:@"matrix_mult(int**, int**)"]) {
                                sd = node.symbolNamePath.count;
                            
                            }
                        }
                    }
                    
                    XRStorageManager *sm = sr.storageManager;

                    /*XRBacktraceRepository *br = sr.backtraceRepository;
                    struct XRTimeRange tr = br.timeRangeFilter;
                    NSLog(@"%llu",tr.length);
                    NSLog(@"%llu", sr.lastTimestamp);
                    NSLog(@"%u", [sm latestEventIdentifierBeforeTimestamp:sr._currentTimestamp]);
                    NSLog(@"%u", sm.lastEventIdentifier);
                    XRCountersEvent *xc = [sm eventForIdentifier:2];
                    NSLog(@"%llu", *(xc.counterValues+3));
                    XRSamplerEvent *xs = (XRSamplerEvent *) xc;
                    NSLog(@"%@", xs.backtrace);
                    XREventDescriptor *ed = [sm eventDescriptor];
                    for (XRFieldDescriptor *e in ed.fields) {
                    NSLog(@"%@", e.name);
                    }
                    XRManagedEvent *xm = [sm eventForIdentifier:2];
                    NSLog(@"%u", xm.identifier);*/
                    
                    
                    XRCountersRun *cr = (XRCountersRun *) run;
                    id i = [cr eventsAndFormulas];

                    for (NSDictionary *l in i) {
                        unsigned long long val = 0;
                        for (int j=0; j<sm.lastEventIdentifier+1; j++) {
                            XRCountersEvent *xc = [sm eventForIdentifier:j];
                            XRSamplerEvent *xs = (XRSamplerEvent *) xc;
                            if (xs.stackDepth == sd) {
                                val += *(xc.counterValues+[[l objectForKey:@"eventsIndex"] intValue]);
                            }
                        }
                        if (verbose) {
                            NSPrint(@"%@",[l objectForKey:@"aliasOrMnemonic"]);
                            NSPrint(@"%llu",val);

                        } else {
                            NSString * valString = [NSString stringWithFormat:@"%llu\n", val];
                            [valString writeToFile:@"/dev/stdout" atomically:NO encoding:NSUTF8StringEncoding error:nil];
                        }
                    }
                }
            }

        }
    }
    return 0;
}
