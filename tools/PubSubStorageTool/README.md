# PubSubStorageTool

The PubSubStorageTool is a companion utility for the files that can be generated using the PUBSUB backend 'STORAGE'
That backend utilized files on disk for recording/capture of pubsub messages and/or their playback/transmission.
Since captures can be non-deterministic if the data source is non-deterministic as to size or quantity of messages it can be hard to gauge ahead of time what the files will look like on disk.
As such this tool allows for manipulating that storage using some pre-canned operations.
parameters as passed as key-value pairs such that you can either place the params in the Main\AppArgs section of the ini config or  you can pass them on the command line
command line parameters will overrule whatever is in the config file. An example command line param syntax would be: 'task=resplit' 'resplitType=time'
In the ini file standard ini format rules apply

## Supported tasks
The tool supports 'tasks' each identified by a name. Each task has its own set of parameters. 
You specify tasks using the 'task' parameter. Here is the list of currently supported tasks:
- resplit

## the 'resplit' task
A repslit takes a set of existing files and re splits them into a new set of files based on a given set of criterea.
This can help to better organize and optimize the storage utilization of the capture files. Such operations are denoted as different resplit types.
You should pass a resplit type to the tool when performing a resplit task. You can do so using the 'resplitType' parameter.

Supported resplit task types are:
- time: Time based container re-splitting. This is the default type.

The common supported parameters for this task are as follows:
- vfsPath: The VFS path where the tool should look for capture files. This is the preferred method
- path: Same as vfsPath except the tool will auto configure a VFS path to link to the system path given via this parameter. If provided it will override vfsPath.
- namePattern: Optional glob-pattern filter param which is used to filter files in the path given. Default filter is "*.pubsubcapture"

### the 'resplit' task of type 'time'
This form of resplit will reorganize the data in the files given to fit within specified time buckets. 
Imagine you have hours of the day where you have only sporadic data and others where you have a lot more data yet in the end you are interested in bucketing the data on an hourly basis
However during the capture itself you need to balance criterea such as recovery in the case of crashes and thus time between flushes to disk, acks to the pubsub system and backing up of data flows that can cause
and more complex situational variables which constrain your freedom to optimally structure your 'data on disk'.
This functionality allows you to optimize the data storage after the fact.

The parameters for this operation are as follows:
- timeBoundary: This can be one of the following: year, month, day, hour, minute, second, millisecond. The default is 'hour'
- targetRootPath: VFS location where the output files are to be created, defaults to the same folder as the input files
- tempRootPath: VFS location where temporary files can be written during the conversion process. Defaults to the system's temp folder
- bestEffortIsOk: boolean flag to indicate whether the tool should attempt to continue best effort even if issues are detected with some of the messages, default is true
- deleteSourceAfterSuccess: boolean flag to indicate whether the tool should attempt to delete the given input files IF the resplit was successfull. Default is false for safety.
