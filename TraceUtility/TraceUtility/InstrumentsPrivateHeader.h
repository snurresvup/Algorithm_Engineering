//
//  InstrumentsPrivateHeader.h
//  TraceUtility
//
//  Created by Qusic on 8/9/15.
//  Copyright (c) 2015 Qusic. All rights reserved.
//

#import <Foundation/Foundation.h>


#ifdef __cplusplus
extern "C" {
#endif
    NSString *PFTDeveloperDirectory(void);
    void PFTLoadPlugins(void);
#ifdef __cplusplus
}
#endif



@interface XRBacktraceAggregator : NSObject
{
    void *_backtraceAndThreadToCounter;
    unsigned long long _resultTimestamp;
    unsigned long long _weightCount;
}

- (id)aggregatedSamplesForPid:(int)arg1;
- (id)aggregatedSamples;
- (void)addBacktraceAggregator:(id)arg1;
- (void)countBacktrace:(id)arg1 thread:(unsigned long long)arg2 withCount:(int)arg3 weights:(double *)arg4;
- (void)dealloc;
- (id)initWithTimestamp:(unsigned long long)arg1 weightCount:(unsigned long long)arg2;
- (id)initWithTimestamp:(unsigned long long)arg1;

@end

struct XRTimeRange {
    unsigned long long start;
    unsigned long long length;
};

@interface PFTCallTreeNode : NSObject
- (NSString *)libraryName;
- (NSString *)symbolName;
- (NSUInteger)address;
- (NSArray *)symbolNamePath; // Call stack
- (PFTCallTreeNode *)root;
- (PFTCallTreeNode *)parent;
- (NSArray *)children;
- (int)numberChildren;
- (id)data;
- (id)symbolData;
- (id)selfBytes;
- (int)uid;
- (id)init;
- (id)_gatherSamples;
- (int)terminals; // An integer value of this node, such as self running time in millisecond.
- (int)count; // Total value of all nodes of the subtree whose root node is this node. It means that if you increase terminals by a value, count will also be increased by the same value, and that the value of count is calculated automatically and you connot modify it.
- (NSUInteger)weightCount; // Count of different kinds of double values;
- (double)selfWeight:(NSUInteger)index; // A double value similar to terminal at the specific index.
- (double)weight:(NSUInteger)index; // A double value similar to count at the specific index. The difference is that you decide how weigh should be calculated.
- (double)selfCountPercent; // self.terminal / root.count
- (double)totalCountPercent; // self.count / root.count
- (double)parentCountPercent; // parent.count / root.count
- (double)selfWeightPercent:(NSUInteger)index; // self.selfWeight / root.weight
- (double)totalWeightPercent:(NSUInteger)index; // self.weight / root.weight
- (double)parentWeightPercent:(NSUInteger)index; // parent.weight / root.weight
@end

@interface XRBacktraceRepository : NSObject
- (id)focusInformation;
- (id)_samplesInSubtreeOfTreeNode:(id)arg1;
- (id)samplesForCategory:(id)arg1;
- (id)backtraceRepository;
- (void)focusSubtree:(id)arg1;
- (id)_leafNameForSample:(id)arg1;
- (id)activeCategory;
@property(nonatomic) BOOL topFunctions;
@property(nonatomic) BOOL showProcessesByCategory;
@property(nonatomic) BOOL showCategoryAsLeaf;
@property(nonatomic) BOOL showAllAsCategory;
@property(nonatomic) BOOL inverted;
@property(nonatomic) BOOL separateByCategory;
@property(nonatomic) BOOL separateByThread;
- (id)filterContext;
- (PFTCallTreeNode *)rootNode;
- (id)boundaryLibraries;
- (void)refreshTreeRoot;
- (id)_trackedTreeData;
- (id)allRepositories;
- (struct XRTimeRange)timeRangeFilter;
- (id)symbolProvider;
@end


@protocol CommonTimeSample <NSCoding, NSObject>
- (id)extendedData;
- (double)weight;
- (int)quantity;
- (unsigned long long)thread;
- (unsigned int)eventType;
- (unsigned long long)timestamp;

@optional
- (double)weightAtIndex:(unsigned int)arg1;
- (unsigned int)cpuIndex;
@end

@protocol CommonRawStack <NSCoding, NSObject>
- (long long)kernelFrameCount;
- (int)pid;
- (long long)count;
- (unsigned long long *)frames;
@end


@interface XRSampleEvent : NSObject <CommonTimeSample>
{
    unsigned long long _timestamp;
    unsigned int _eventType;
    int _quantity;
    unsigned long long _thread;
    id _extendedData;
    unsigned long long _weightCount;
    union {
        double scalar;
        double *array;
    } _weight;
}

+ (void)initialize;
- (id)extendedData;
- (id)backtrace;
- (int)quantity;
- (double)weightAtIndex:(unsigned int)arg1;
- (double)weight;
- (unsigned long long)thread;
- (unsigned int)eventType;
- (unsigned long long)timestamp;
- (id)initWithCoder:(id)arg1;
- (void)encodeWithCoder:(id)arg1;
- (void)dealloc;
- (id)initWithTimestamp:(unsigned long long)arg1 eventType:(unsigned int)arg2 thread:(unsigned long long)arg3 weights:(double *)arg4 weightCount:(unsigned long long)arg5 quantity:(int)arg6 backtrace:(id)arg7 extendedData:(id)arg8;

// Remaining properties
@property(readonly, copy) NSString *debugDescription;
@property(readonly, copy) NSString *description;
@property(readonly) unsigned long long hash;
@property(readonly) Class superclass;

@end


@interface XRRun : NSObject
{
    
}
- (NSInteger)runNumber;
- (NSString *)displayName;
- (NSTimeInterval)startTime;
- (NSTimeInterval)endTime;
@end

@interface XREventDescriptor : NSObject <NSCoding>
@property(readonly) NSMutableDictionary *properties; // @synthesize properties=_publicProps;
@property(readonly) NSArray *fields; // @synthesize fields=_fields;
@property(readonly) Class eventClass; // @synthesize eventClass=_eventClass;

@end

@interface XRFieldDescriptor : NSObject <NSCoding>
+ (int)fieldTypeForString:(id)arg1;
@property(readonly) NSString *name; // @synthesize name=_name;
@property(readonly) int type; // @synthesize type=_type;
@property(readonly) unsigned int size;
@property(readonly) unsigned int maxMultiples;
@property(readonly) unsigned int identifierHandle;
@property(readonly) unsigned int handle;
@property(readonly) BOOL isMultiField;
@end

@interface XRStorageManager : NSObject
@property(copy, nonatomic) NSString *filePath; // @synthesize filePath=_filePath;
- (id)eventForIdentifier:(unsigned int)arg1;
- (unsigned int)latestEventIdentifierBeforeTimestamp:(unsigned long long)arg1;
@property(readonly, nonatomic) XREventDescriptor *eventDescriptor;
- (unsigned int)lastEventIdentifier;
@end

@interface XRSamplerRun : XRRun
@property(nonatomic) unsigned long long pmiEventThreshold; // @synthesize pmiEventThreshold=_pmiEventThreshold;
@property(retain, nonatomic) NSString *pmiCounterName; // @synthesize pmiCounterName=_pmiCounterName;
@property(retain, nonatomic) NSString *pmiEventMnemonic; // @synthesize pmiEventMnemonic=_pmiEventMnemonic;
@property(nonatomic) unsigned int displayedEvent; // @synthesize displayedEvent=_displayedEvent;
@property(readonly) XRStorageManager *storageManager;
- (XRBacktraceRepository *)backtraceRepository;
- (id)eventForIdentifier:(unsigned int)arg1;
- (unsigned int)closestEventIdentifierAfterTimestamp:(unsigned long long)arg1;
- (id)processNameForPid:(int)arg1;
@property(readonly) NSArray *allThreads;
- (id)threadForEvent:(id)arg1;
@property(readonly) unsigned long long lastTimestamp;
@end

@interface XRRunListData : NSObject
- (NSDictionary *)runData;
- (NSArray *)runNumbers;
- (NSDictionary *)dataForAllRuns;
@end

@interface PFTInstrumentType : NSObject
- (NSString *)uuid;
- (NSString *)name;
- (NSString *)category;
@end

@interface XRDevice : NSObject
- (NSString *)deviceIdentifier;
- (NSString *)deviceDisplayName;
- (NSString *)deviceDescription;
- (NSString *)productType;
- (NSString *)productVersion;
- (NSString *)buildVersion;
@end

@interface PFTProcess : NSObject
- (NSString *)bundleIdentifier;
- (NSString *)displayName;
- (NSString *)executablePath;
- (NSArray *)arguments;
- (NSDictionary *)environment;
- (XRDevice *)device;
@end

@interface XRInstrument : NSObject
- (PFTInstrumentType *)type;
- (PFTProcess *)target;
- (NSArray *)allRuns;
@end

@interface PFTInstrumentList : NSObject
- (NSArray *)allInstruments;
@end

@interface XRTrace : NSObject
- (instancetype)initForCommandLine:(BOOL)commandLine;
- (PFTInstrumentList *)basicInstruments;
- (PFTInstrumentList *)recordingInstruments;
- (XRRunListData *)runData;
- (BOOL)saveDocument:(NSURL *)documentURL saveAllRuns:(BOOL)saveAllRuns error:(NSError **)errpt;
- (BOOL)loadDocument:(NSURL *)documentURL error:(NSError **)errpt;
- (void)close;
@end

@interface XRCountersRun : XRSamplerRun
{
    XRDevice *_device;
    unsigned long long _updateRate;
    BOOL _pidCaching;
    BOOL _missingEntryWarning;
    NSObject<OS_dispatch_queue> *_workQueue;
    void *_queuedEvents;
    unsigned int _queuedSize;
    unsigned int _queuedCount;
    unsigned long long _markTimestamp;
    unsigned long long _latestTimestamp;
    NSMutableArray *_events;
    NSMutableArray *_formulas;
    NSMutableArray *_formulaEvaluators;
    NSMutableArray *_eventsAndFormulas;
    NSMutableArray *_eventsThenFormulas;
    unsigned long long _numberOfEventAndFormulaGraphsCached;
    unsigned long long *_graphIndexRemapping;
    unsigned int _timestampHandle;
    unsigned int _tidHandle;
    unsigned int _backtraceHandle;
    unsigned int _backtraceIDHandle;
    unsigned int _runningTimeHandle;
    unsigned int _cpuHandle;
    unsigned int _threadStateHandle;
    unsigned int _counterValuesHandle;
}

+ (void)initialize;
@property(nonatomic) unsigned long long updateRate; // @synthesize updateRate=_updateRate;
- (void)markAndFlushQueue:(BOOL)arg1;
- (void)_queueEventWithTimestamp:(unsigned long long)arg1 thread:(unsigned long long)arg2 backtrace:(unsigned int)arg3 cpu:(unsigned int)arg4 threadState:(unsigned int)arg5 counterValues:(unsigned long long *)arg6;
- (id)eventDescriptorForIdentifier:(id)arg1;
- (BOOL)loadDTPerformanceSessionDataFromPaths:(id)arg1 error:(id *)arg2;
- (unsigned long long)eventIndexForEventOrFormulaIndex:(unsigned long long)arg1;
- (double)evaluateFormulaAtIndex:(unsigned long long)arg1 withEventArray:(double *)arg2 arrayCount:(unsigned long long)arg3;
- (BOOL)isFormulaAtEventOrFormulaIndex:(unsigned long long)arg1;
- (unsigned long long)eventOrFormulaIndexForDisplayedIndex:(unsigned long long)arg1;
- (unsigned long long)numberOfGraphs;
@property(readonly, nonatomic) unsigned long long formulasCount;
@property(readonly, nonatomic) unsigned long long eventsCount;
@property(readonly, nonatomic) unsigned long long eventsAndFormulasCount;
- (id)formulaEvaluatorAtIndex:(unsigned int)arg1;
- (id)eventsAndFormulas;
- (void)setCountersFromModel:(id)arg1;
- (void)processRecord:(struct _CPRecord *)arg1 enqueue:(BOOL)arg2;
- (void)prepareForDevice:(id)arg1 options:(int)arg2 pid:(int)arg3;
- (id)backtracesForCategory:(id)arg1 timeRange:(struct XRTimeRange)arg2 savedIndex:(unsigned long long *)arg3;
- (id)backtraceRepository;
- (unsigned long long)weightCount;
- (unsigned long long)_currentTimestamp;
- (BOOL)stopRecordingWithPid:(int)arg1;
- (BOOL)startRecordingWithPid:(int)arg1;
- (Class)decoratorForStrategy:(id)arg1;
- (id)workQueue;
- (id)initWithCoder:(id)arg1;
- (id)_storagePathForVersion:(long long)arg1;
- (void)encodeWithCoder:(id)arg1;
- (id)init;
@end


@interface XRRawBacktrace : NSObject <CommonRawStack>
{
    unsigned long long *_frames;
    unsigned int _count;
    int _pid;
    unsigned long long _hash;
}

- (BOOL)backtraceIsEqual:(id)arg1;
- (unsigned long long)backtraceHash;
- (int)pid;
- (long long)kernelFrameCount;
- (long long)count;
- (unsigned long long *)frames;
- (id)initWithDataStream:(char *)arg1 position:(unsigned long long *)arg2 max:(unsigned long long)arg3 version:(long long)arg4 substitutions:(struct DataRepresentationSubstitutions *)arg5;
- (void)addToDataStream:(char *)arg1 position:(unsigned long long *)arg2 max:(unsigned long long)arg3 substitutions:(struct DataRepresentationSubstitutions *)arg4;
- (id)initWithCoder:(id)arg1;
- (void)encodeWithCoder:(id)arg1;
- (void)setFrames:(unsigned long long *)arg1 count:(unsigned int)arg2;
- (void)dealloc;
- (id)initWithFrames:(unsigned long long *)arg1 count:(long long)arg2 pid:(int)arg3;

// Remaining properties
@property(readonly, copy) NSString *debugDescription;
@property(readonly, copy) NSString *description;
@property(readonly) unsigned long long hash;
@property(readonly) Class superclass;

@end


@interface XRManagedEvent : NSObject <NSCopying>
{
    int _managedRetainCount;
    unsigned int _identifier;
}

+ (void)configureDescriptor:(id)arg1 forStorageManager:(id)arg2;
- (id)copyWithZone:(struct _NSZone *)arg1;
- (unsigned long long)hash;
- (BOOL)isEqual:(id)arg1;
- (unsigned long long)retainCount;
- (oneway void)release;
- (id)retain;
- (unsigned int)_managedRetainCount;
- (id)storageManager;
- (unsigned int)identifier;

@end

@interface XRSamplerEvent : XRManagedEvent
{
    unsigned long long _timestamp;
    unsigned long long _tid;
    id <CommonRawStack> _backtrace;
    unsigned int _cpu;
    unsigned int _threadState;
    unsigned long long _runningTime;
}

+ (void)configureDescriptor:(id)arg1 forStorageManager:(id)arg2;
@property(readonly) id <CommonRawStack> backtrace; // @synthesize backtrace=_backtrace;
@property(readonly) unsigned int cpuIndex; // @synthesize cpuIndex=_cpu;
@property(readonly) unsigned long long tid; // @synthesize tid=_tid;
@property(readonly) unsigned long long timestamp; // @synthesize timestamp=_timestamp;
- (id)initWithCoder:(id)arg1;
- (void)encodeWithCoder:(id)arg1;
@property(readonly) unsigned int stackDepth;
@property(readonly) unsigned long long runningTime;

@end


@interface XRCountersEvent : XRSamplerEvent
{
    unsigned long long _eventsAndFormulasCount;
    unsigned long long *_counterValues;
}

+ (void)configureDescriptor:(id)arg1 forStorageManager:(id)arg2;
@property(readonly) double *counterValuesAsDouble;
@property(readonly) unsigned long long *counterValues;
- (void)dealloc;

@end