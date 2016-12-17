//
//  MyBlueToothManager.h
//  ShiSu
//
//  Created by 郑小燕 on 16/12/6.
//  Copyright © 2016年 simpleway. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>

typedef NS_ENUM(NSInteger, OptionStage) {
    OptionStageConnection,            //蓝牙连接阶段
    OptionStageSeekServices,          //搜索服务阶段
    OptionStageSeekCharacteristics,   //搜索特性阶段
    OptionStageSeekdescriptors,        //搜索描述信息阶段
};

#pragma mark ------------------- 通知的定义 --------------------------
/** 蓝牙状态改变的通知 */
#define kCentralManagerStateUpdateNoticiation @"kCentralManagerStateUpdateNoticiation"
#pragma mark ------------------- block的定义 --------------------------

/** 蓝牙状态改变的block */
typedef void(^MyStateUpdateBlock)(CBCentralManager * centralManager);
/**
 *  发现一个蓝牙外设的block
 */
typedef void(^MyDiscoverPeripheralBlock)(CBCentralManager *centralManager,CBPeripheral *periphera,NSDictionary *advertisementData,NSNumber *RSSI);

/**
 * 搜索到连接上的蓝牙外设的服务block
 */
typedef void(^MyDiscoveredServicesBlock)(CBPeripheral *peripheral,NSArray *services,NSError *error);


/**
 * 统一返回使用的block
 */
typedef void(^MyBLECompletionBlock)(OptionStage stage,CBPeripheral *peripheral,CBService *service, CBCharacteristic *character, NSError *error);

/**
 * 搜索某个服务的子服务 的回调
 */

typedef void(^MyDiscoveredIncludedServicesBlock)(CBPeripheral *peripheral,CBService *service, NSArray *includedServices, NSError *error);

/**
 * 搜索到某个服务中的特性的block
 */
typedef void(^MyDiscoverCharacteristicsBlock)(CBPeripheral *peripheral, CBService *service, NSArray *characteristics, NSError *error);


/**
 * 收到某个特性值更新的回调
 */

typedef void(^MyNotifyCharacteristicBlock)(CBPeripheral *peripheral,CBCharacteristic *characteristic,NSError *error);



/**
 * 获取特性中的值
 */

typedef void(^MyValueForCharacteristicBlock)(CBCharacteristic *characteristic, NSData *value, NSError *error);


/**
 * 获取描述值的回调
 */

typedef void(^MyValueForDescriptorBlock)(CBDescriptor *descriptor,NSData *data,NSError *error);

/**
 * 往描述中写入数据的回调
 */

typedef void(^MyWriteToDescriptorBlock)(CBDescriptor *descriptor, NSError *error);


/**
 * 往特性中写入数据的回调
 */
typedef void(^MyWriteToCharacteristicBlock)(CBCharacteristic *characteristic, NSError *error);


/**
 * 获取蓝牙外设信号的回调
 */
typedef void(^MyGetRSSIBlock)(CBPeripheral *peripheral,NSNumber *RSSI, NSError *error);






@interface MyBlueToothManager : NSObject
/**
 * 每次发送的最大数据长度，因为部分型号的蓝牙打印机一次写入数据过长，会导致打印乱码。
 * iOS 9之后，会调用系统的API来获取特性能写入的最大数据长度。
 * 但是iOS 9之前需要自己测试然后设置一个合适的值。默认值是146，我使用佳博58MB-III的限度。
 * 所以，如果你打印乱码，你考虑将该值设置小一点再试试。
 */
@property (assign, nonatomic)   NSInteger             limitLength;

/**
 * 蓝牙模块状态改变的回调
 */
@property (nonatomic, strong) MyStateUpdateBlock stateUpdateBlock;
/**
 * 发现一个蓝牙外设的block
 */

@property (nonatomic, strong) MyDiscoverPeripheralBlock discoverPeripheralBlcok;

/**
 * 当前连接的外设
 */
@property (strong, nonatomic, readonly)   CBPeripheral            *connectedPerpheral;

/**
 * 发现服务的回调
 */

@property (nonatomic, strong) MyDiscoveredServicesBlock discoverServicesBlock;


/**
 * 操作完成的统一回调
 */
@property (nonatomic, strong) MyBLECompletionBlock completionBlock;

/**
 * 发现服务中的子服务的回调
 */

@property (nonatomic, strong) MyDiscoveredIncludedServicesBlock discoverdIncludedServicesBlock;


/**
 * 发现服务中的特性的回调
 */
@property (nonatomic, strong) MyDiscoverCharacteristicsBlock discoverCharacteristicsBlock;


/**
 * 特性值改变的回调
 */
@property (nonatomic, strong) MyNotifyCharacteristicBlock notifyCharacteristicBlock;

/**
 * 获取特性值回调 
 */
@property (nonatomic, strong) MyValueForCharacteristicBlock valueForCharacteristicBlock;

/**
 * 获取描述值的回调
 */

@property (nonatomic, strong) MyValueForDescriptorBlock valueForDescriptorBlock;


/**
 * 将数据写入描述中的回调
 */
@property (nonatomic, strong) MyWriteToDescriptorBlock writeToDescriptorBlock;

/**
 * 将数据写入特性中的回调
 */
@property (nonatomic, strong) MyWriteToCharacteristicBlock writeToCharacteristicBlock;

/**
 * 获取蓝牙外设信号强度的回调
 */
@property (nonatomic, strong) MyGetRSSIBlock getRSSIBlock;


#pragma mark - method
+(instancetype)sharedInstance;
/**
 *  开始搜索蓝牙外设，每次在block中返回一个蓝牙外设信息
 *
 *  @param uuids         服务的CBUUID
 *  @param options        其他可选参数
 */
- (void)scanForPeripheralsWithServiceUUIDs:(NSArray<CBUUID *> *)uuids options:(NSDictionary<NSString *, id> *)options;

/**
 *  开始搜索蓝牙外设，每次在block中返回一个蓝牙外设信息
 *  返回的block参数可参考CBCentralManager 的 centralManager:didDiscoverPeripheral:advertisementData:RSSI:
 *
 *  @param uuids         服务的CBUUID
 *  @param options        其他可选参数
 *  @param discoverBlock 搜索到蓝牙外设后的回调
 */

- (void)scanForPeripheralsWithServiceUUIDs:(NSArray<CBUUID *> *)uuids options:(NSDictionary<NSString *, id> *)options didDiscoverPeripheral:(MyDiscoverPeripheralBlock)discoverBlock;
/**
 *  连接某个蓝牙外设，并查询服务，特性，特性描述
 *
 *  @param peripheral          要连接的蓝牙外设
 *  @param connectOptions      连接的配置参数
 *  @param stop                连接成功后是否停止搜索蓝牙外设
 *  @param serviceUUIDs        要搜索的服务UUID
 *  @param characteristicUUIDs 要搜索的特性UUID
 *  @param completionBlock     操作执行完的回调
 */


- (void)connectPeripheral:(CBPeripheral *)peripheral
           connectOptions:(NSDictionary<NSString *,id> *)connectOptions
   stopScanAfterConnected:(BOOL)stop
          servicesOptions:(NSArray<CBUUID *> *)serviceUUIDs
   characteristicsOptions:(NSArray<CBUUID *> *)characteristicUUIDs
            completeBlock:(MyBLECompletionBlock)completionBlock;


/**
 *  查找某个服务的子服务
 *
 *  @param includedServiceUUIDs 要查找的子服务的UUIDs
 *  @param service              父服务
 */
- (void)discoverIncludedServices:(NSArray<CBUUID *> *)includedServiceUUIDs forService:(CBService *)service;

/**
 *  读取某个特性的值
 *
 *  @param characteristic 要读取的特性
 */
- (void)readValueForCharacteristic:(CBCharacteristic *)characteristic;

/**
 *  读取某个特性的值
 *
 *  @param characteristic  要读取的特性
 *  @param completionBlock 读取完后的回调
 */
- (void)readValueForCharacteristic:(CBCharacteristic *)characteristic completionBlock:(MyValueForCharacteristicBlock)completionBlock;

/**
 *  往某个特性中写入数据
 *
 *  @param data           写入的数据
 *  @param characteristic 特性对象
 *  @param type           写入类型
 */
- (void)writeValue:(NSData *)data forCharacteristic:(CBCharacteristic *)characteristic type:(CBCharacteristicWriteType)type;


/**
 *  往某个特性中写入数据
 *
 *  @param data           写入的数据
 *  @param characteristic 特性对象
 *  @param type           写入类型
 *  @param completionBlock 写入完成后的回调,只有type为CBCharacteristicWriteWithResponse时，才会回调
 */
- (void)writeValue:(NSData *)data forCharacteristic:(CBCharacteristic *)characteristic type:(CBCharacteristicWriteType)type completionBlock:(MyWriteToCharacteristicBlock)completionBlock;

/**
 *  读取某特性的描述信息
 *
 *  @param descriptor 描述对象
 */
- (void)readValueForDescriptor:(CBDescriptor *)descriptor;

/**
 *  读取某特性的描述信息
 *
 *  @param descriptor      描述对象
 *  @param completionBlock 读取结果返回时的回调
 */
- (void)readValueForDescriptor:(CBDescriptor *)descriptor completionBlock:(MyValueForDescriptorBlock)completionBlock;

/**
 *  将数据写入特性的描述中
 *
 *  @param data       数据
 *  @param descriptor 描述对象
 */
- (void)writeValue:(NSData *)data forDescriptor:(CBDescriptor *)descriptor;

/**
 *  将数据写入特性的描述中
 *
 *  @param data       数据
 *  @param descriptor 描述对象
 *  @param completionBlock 数据写入完成后的回调
 */
- (void)writeValue:(NSData *)data forDescriptor:(CBDescriptor *)descriptor completionBlock:(MyWriteToDescriptorBlock)completionBlock;

/**
 *  获取某外设的信号
 *
 *  @param getRSSIBlock 获取信号完成后的回调
 */
- (void)readRSSICompletionBlock:(MyGetRSSIBlock)getRSSIBlock;

/**
 *  停止扫描
 */
- (void)stopScan;

/**
 *  断开蓝牙连接
 */
- (void)cancelPeripheralConnection;


@end
