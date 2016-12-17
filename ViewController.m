//
//  ViewController.m
//  BlueTooth
//
//  Created by 郑小燕 on 16/12/8.
//  Copyright © 2016年 郑小燕. All rights reserved.
//

#import "ViewController.h"
#import "MyBlueToothManager.h"

@interface ViewController ()<UITableViewDelegate,UITableViewDataSource>
@property (nonatomic, strong) UITableView *tableView;
@property (strong, nonatomic)   CBCharacteristic            *chatacter;  /**< 可写入数据的特性 */
@end

@implementation ViewController{
    MyBlueToothManager *manager;
    NSMutableArray *dataArray;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    dataArray = [NSMutableArray array];
   
 [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"PersonalInformationViewController"];
    manager = [MyBlueToothManager sharedInstance];
    manager.stateUpdateBlock = ^(CBCentralManager *central){
        
                NSString *info = nil;
                switch (central.state) {
                    case CBManagerStatePoweredOn:
                        info = @"蓝牙已打开,并且可用";
//                        [manager scanForPeripheralsWithServiceUUIDs:nil options:nil];
                        [self scan];
                        NSLog(@"蓝牙已打开,并且可用");
                        break;
                    case CBCentralManagerStatePoweredOff:
                        info = @"蓝牙可用,未打开";
                        NSLog(@"蓝牙可用,未打开");
                        break;
                    case CBCentralManagerStateUnsupported:
                        info = @"SDK不支持";
                        NSLog(@"SDK不支持");
                        break;
                    case CBCentralManagerStateUnauthorized:
                        info = @"程序未授权";
                        NSLog(@"程序未授权");
                        break;
                    case CBCentralManagerStateResetting:
                        info = @"CBCentralManagerStateResetting";
                        NSLog(@"CBCentralManagerStateResetting");
                        break;
                    case CBCentralManagerStateUnknown:
                        info = @"CBCentralManagerStateUnknown";
                        break;
                        
                    default:
                        break;
                }
            
                
                
            };

   }

- (void)scan{
    [manager scanForPeripheralsWithServiceUUIDs:nil options:nil didDiscoverPeripheral:^(CBCentralManager *centralManager, CBPeripheral *periphera, NSDictionary *advertisementData, NSNumber *RSSI) {
        NSDictionary *dic = @{@"periphera":periphera,@"advertisementData":advertisementData,@"RSSI":RSSI};
        NSLog(@"RSSI= %@ advertisementData==%@ periphera= %@",RSSI,advertisementData,periphera);
        [dataArray addObject:dic];
        [self.tableView reloadData];
        
        
        
    }];

}

- (UITableView *)tableView{
    if (!_tableView) {
        _tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 64) style:UITableViewStyleGrouped];
//        _tableView.backgroundColor= [UIColor redColor];
        _tableView.delegate = self;
        [self.view addSubview:_tableView];
        _tableView.dataSource = self;
       
        
        
    }
    
    return _tableView;
    
    
    
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    
    return dataArray.count;
}
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    
    return 1;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"PersonalInformationViewController"];
    NSDictionary *dic = dataArray[indexPath.row];
    cell.textLabel.text = [dic[@"periphera"] name];
    cell.detailTextLabel.text = dic[@"RSSI"];
    
    return cell;
    
    
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    return 50 ;
}
- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    return 0.001;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 0.01;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSDictionary *dic = dataArray[indexPath.row];
    [manager connectPeripheral:dic[@"periphera"] connectOptions:nil stopScanAfterConnected:NO servicesOptions:nil characteristicsOptions:nil completeBlock:^(OptionStage stage, CBPeripheral *peripheral, CBService *service, CBCharacteristic *character, NSError *error) {
       
        if (error) {
            
            NSLog(@"连接失败");
            
        } else {
            
            NSLog(@"连接成功");
            NSLog(@"=======peripheral =%@,service = %@  peripheral.services =  %@ character = %@=========",peripheral,service,peripheral.services,character);
            
            for (CBService *myserverce in peripheral.services) {
                
                NSLog(@"myserverce = %@",myserverce);
                for (CBCharacteristic *cbCharacteristic in myserverce.characteristics) {
                    NSLog(@"service.characteristics = %@ +++++++++++CBCharacteristic = %@",myserverce.characteristics,cbCharacteristic);
                   
                    CBCharacteristicProperties properties = cbCharacteristic.properties;
                    NSLog(@"+++++++++++CBCharacteristicProperties = %ld+++++++++",properties);
                    
                    if (properties & CBCharacteristicPropertyBroadcast) {
                        NSLog(@"CBCharacteristicPropertyBroadcast");
                    }
                    if (properties & CBCharacteristicPropertyRead) {
                        NSLog(@"CBCharacteristicPropertyRead");
                    }
                    if (properties & CBCharacteristicPropertyWriteWithoutResponse) {
                        NSLog(@"CBCharacteristicPropertyWriteWithoutResponse");
                    }
                    if (properties & CBCharacteristicPropertyWrite) {
                        NSLog(@"CBCharacteristicPropertyWrite");
                        NSString *str = @" 1、订票提醒:门票支持多订少取,游客按实际人数到景区付款购票;团队电话:023-61918888.";
                        NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
                        NSData *data = [str dataUsingEncoding:enc];
                                    [manager writeValue:data forCharacteristic:cbCharacteristic type:CBCharacteristicWriteWithoutResponse];

                    }
                    if (properties & CBCharacteristicPropertyNotify) {
                        NSLog(@"CBCharacteristicPropertyNotify");
                    }
                    if (properties & CBCharacteristicPropertyIndicate) {
                        NSLog(@"CBCharacteristicPropertyIndicate");
                    }
                    if (properties & CBCharacteristicPropertyAuthenticatedSignedWrites) {
                        NSLog(@"CBCharacteristicPropertyAuthenticatedSignedWrites");
                    }
                    if (properties & CBCharacteristicPropertyExtendedProperties) {
                        NSLog(@"CBCharacteristicPropertyExtendedProperties");
                    }
                    if (properties & CBCharacteristicPropertyNotifyEncryptionRequired) {
                        NSLog(@"CBCharacteristicPropertyNotifyEncryptionRequired");
                    }
                    if (properties & CBCharacteristicPropertyIndicateEncryptionRequired) {
                        NSLog(@"CBCharacteristicPropertyIndicateEncryptionRequired");
                    }
//                    if (properties & CBCharacteristicPropertyWrite) {
//                       
//                        self.chatacter = character;
//                    }

                }
            }
                   }
    }];
    
}




- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
