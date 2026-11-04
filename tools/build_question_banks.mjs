import fs from 'node:fs/promises';
import path from 'node:path';
import { Workbook, SpreadsheetFile } from '@oai/artifact-tool';

const root = path.resolve(import.meta.dirname, '..');
const pictures = (await fs.readdir(path.join(root, 'pitctures')))
  .filter(name => name.includes('a935a6faaffa') && name.endsWith('.jpg')).sort();
const outputDir = path.join(root, 'outputs', 'embedded_quiz_20261103');
await fs.mkdir(outputDir, { recursive: true });

// [original number, question, choices, answer, explanation, screenshot index]
const single = [
  [2,'STM32F103ZET6是单片机（）。',['16','8','32','64'],'C','STM32F103ZET6采用32位ARM Cortex-M3内核。',0],
  [3,'Cortex-M处理器采用的架构是（）。',['v4T','v5TE','v6','v7'],'D','本题针对Cortex-M3，采用ARMv7-M架构；“Cortex-M”泛指整个系列时题干不够严谨。',1],
  [4,'PWM是（）。',['脉冲宽度调制','脉冲频率调制','脉冲幅度调制','脉冲位置调制'],'A','PWM是Pulse Width Modulation，即脉冲宽度调制。',2],
  [5,'关于中断嵌套说法正确的是（）。',['只要响应优先级不一样就有可能发生中断嵌套','只要抢占式优先级不一样就有可能发生中断嵌套','只有抢占式优先级和响应优先级都不一样才有可能发生中断嵌套','以上说法都不对'],'B','可抢占优先级较高的中断能够打断较低优先级的中断服务程序。',3],
  [6,'下面是Cortex-M3处理器的工作模式的是（）。',['Thread模式','Thumb模式','Thumb-2模式','Debug模式'],'A','Cortex-M3有线程模式与处理模式；Thumb属于指令集状态。',4],
  [7,'所有的GPIO引脚有内部弱上拉和下拉，当它们被配置为（）时可以是激活的或者非激活的。',['输入','输出','推挽','开漏'],'A','输入模式可配置为上拉、下拉或浮空。',5],
  [8,'每个I/O端口位可以自由地编程，尽管I/O端口寄存器必须以哪种方式访问（）。',['16位字','16位字节','32位字节','32位字'],'D','STM32F1的GPIO寄存器按32位字访问。',8],
  [9,'在GPIO控制LED电路设计时，若最大输出速度为2MHz，MODE[1:0]应设置为（）。',['00','01','10','11'],'C','STM32F1 GPIO输出模式编码：10表示最大速度2MHz。',9],
  [10,'每个通用I/O端口有几个32位的配置寄存器、几个32位的数据寄存器、几个32位的置位/复位寄存器、几个16位的复位寄存器、几个32位的锁定寄存器（）。',['2,1,2,1,1','2,2,1,1,1','2,2,2,1,1','2,2,1,2,1'],'B','CRL/CRH共2个，IDR/ODR共2个，BSRR 1个，BRR 1个，LCKR 1个。',10],
  [11,'STM32F103RCT6具有GPIO引脚数为（）。',['50','51','52','53'],'B','RC封装为64引脚，GPIO可用引脚数为51。',11],
  [12,'在GPIO控制LED电路设计时，若最大输出速度为10MHz，应设置（）。',['CNFy[1:0]','MODEy[1:0]','MODE','CNF'],'B','GPIO的MODEy[1:0]设置输出速度，01对应10MHz。',12],
  [13,'STM32普通定时器除基本定时器功能外，还具备（）等功能。',['输入捕获、输出比较、单脉冲输出、PWM信号输出、正交编码器','输出捕获、输出比较、单脉冲输出、PWM信号输出、正交编码器','输入捕获、输出计数、单脉冲输出、PWM信号输出、正交编码器','输入捕获、输出比较、多脉冲输出、AWN信号输出、正交编码器'],'A','通用定时器支持输入捕获、输出比较、单脉冲、PWM及编码器接口。',13],
  [14,'STM32定时器主要由（）决定。',['DSC、ARR、FCLK','PSC、ARR、FCLK','PSC、BRR、FCLK','PSC、ARR、CLK'],'B','计数周期由输入时钟频率、预分频器PSC和自动重装载ARR决定。',14],
  [15,'STM32F103系列单片机不具有PWM功能的是（）。',['TIM1','TIM2','TIM3','TIM6'],'D','TIM6是基本定时器，没有输出比较/PWM通道。',15],
  [16,'当外围器件需要CPU执行操作时发出中断请求，CPU暂停当前工作，执行（）处理请求，完成后返回断点。',['保护现场，子程序服务','保护现场，中断服务程序','保存断点，中断服务程序','保护断点，子程序服务'],'B','中断响应先保护现场，随后执行中断服务程序。',16],
  [17,'Cortex-M3一共支持（）个中断。',['254','255','256','257'],'C','架构包含16个内核异常和最多240个外部中断，共256个异常/中断编号。',17],
  [18,'Cortex-M3包含16个内部中断与240个外部可屏蔽中断，其中（）个内核中断厂家无法修改。',['16','17','18','19'],'A','16个系统异常/内核中断由内核架构定义。',18],
  [19,'STM32F103RCT6仅有外部可屏蔽中断个数为（）。',['55','56','59','60'],'D','该型号实现60个外部中断通道。',19],
  [20,'高优先级中断可以打断正在执行的低优先级中断服务程序，待高优先级中断完成后再继续执行低优先级中断服务程序，这就是（）。',['中断优先权','中断请求','中断嵌套','中断服务'],'C','题干描述的是中断嵌套。',20],
  [21,'异步通信中数据通常以（）为最小单位组成数据帧传送。',['字','双字','字节','4位'],'C','UART通常按一个字符/字节组成的数据帧逐帧发送。',21],
  [22,'起始位位于数据帧开头，仅占（）位，为逻辑“0”信号。',['4','3','2','1'],'D','UART起始位固定为1位低电平。',22],
  [23,'数据位位于起始位之后，可取8位或9位，发送顺序为（）。',['低位在前高位在后','低位在后高位在前','低位高位随机','低位高位并行'],'A','UART按LSB先行发送。',23],
  [24,'STM32F103系列单片机不具有PWM功能的是（）。',['TIM1','TIM2','TIM3','TIM6'],'D','与原题15重复，已保留原题号便于回查；TIM6没有PWM通道。',24],
  [26,'用来允许对GPIO寄存器进行原子的读/修改操作的寄存器是（）。',['GPIOx_BSRR和GPIOx_BRR','GPIOx_CRL和GPIOx_CRH','GPIOx_BSRR和GPIOx_LCKR','GPIOx_IDR和GPIOx_ODR'],'A','BSRR/BRR可在不干扰其他位的情况下置位或复位输出。',26],
  [27,'固件库中的功能状态（FunctionalState）类型被赋予以下两个值（）。',['ENABLE或者DISABLE','SET或者RESTE','YES或者NO','SUCCESS或者ERROR'],'A','FunctionalState枚举为DISABLE和ENABLE。',27],
  [28,'固件库中的标志状态（FlagStatus）类型被赋予以下两个值（）。',['ENABLE或者DISABLE','SUCCESS或者ERROR','SET或者RESTE','YES或者NO'],'C','FlagStatus枚举为RESET和SET；截图将RESET误写为RESTE，保留原选项。',28],
  [29,'STM32F103ZET6编排中断优先级的位数为（）。',['4','8','16','32'],'A','STM32F103实现4位NVIC中断优先级。',29],
  [30,'向量中断控制器最多可支持IRQ中断个数为（）。',['127','128','240','255'],'C','Cortex-M3 NVIC架构最多支持240个外部IRQ。',30],
  [31,'系统控制寄存器、NVIC和处理器内核接口紧密耦合，主要目的是（）。',['结构更紧凑，减小芯片的尺寸','连接更可靠，减小出错的概率','减小延时，高效处理最近发生的中断','无所谓，没有特别的意思，远一点也没有关系'],'C','紧密耦合可降低中断响应延迟。',31],
  [32,'关于中断嵌套说法正确的是（）。',['只要响应优先级不一样就有可能发生中断嵌套','只要抢占式优先级不一样就有可能发生中断嵌套','只有抢占式优先级和响应优先级都不一样才有可能发生中断嵌套','以上说法都不对'],'B','与原题5重复；嵌套由抢占优先级决定。',32],
  [33,'在STM32F103ZET6向量中断控制器管理下，可将中断分组个数为（）。',['4','5','6','7'],'B','STM32标准外设库提供NVIC_PriorityGroup_0至4，共5种分组。',33],
  [34,'要使能自动重装载的预装载寄存器，需设置TIMx_CR1寄存器位为（）。',['UIF','ARPE','UG','URS'],'B','CR1中的ARPE位控制自动重装载预装载。',34],
  [35,'若WKUP连接了STM32F10x的PA0 GPIO，PA0映射到外部中断事件线是（）。',['EXTI线0','EXTI线1','EXTI线2','EXTI线3'],'A','GPIO的第0号引脚映射至EXTI0。',35],
];

const multiple = [
  [36,'GPIO的八种工作模式是推挽输出、开漏输出、复用推挽输出、复用开漏输出以及（）。',['模拟量输入','浮空输入','下拉输入','上拉输入'],'ABCD','剩余四种输入模式是模拟、浮空、上拉和下拉输入。',36],
  [37,'推挽输出是指（）。',['当控制输出1时，P-MOS管闭合、N-MOS管断开，引脚输出电源电压（+3.3V）','当控制输出0时，P-MOS管断开、N-MOS管闭合，引脚输出GND','当控制输出0时，P-MOS管闭合、N-MOS管断开，引脚输出GND','当控制输出1时，P-MOS管断开、N-MOS管闭合，引脚输出电源电压（+3.3V）'],'AB','推挽级由上、下两个晶体管交替导通。',37],
  [38,'PWM技术一般可用于哪些领域（）。',['直流电机调速','开关电源','LED亮度调节','继电器'],'ABC','前三者是常见PWM调节用途；继电器可在特定驱动设计中使用PWM保持电流，但一般开关控制不按此归类。',38],
  [39,'STM32F103RCT6具有51个GPIO的4种输入模式为（）。',['模拟量输入','浮空输入','下拉输入','上拉输入'],'ABCD','GPIO输入可配置为模拟、浮空、上拉、下拉。',40],
  [40,'外部可屏蔽中断除了EXTI之外，还有哪些中断（）。',['定时器中断','串口通信中断','模数转换中断','SPI中断'],'ABCD','定时器、USART、ADC、SPI均可产生外设中断。',41],
  [41,'STM32的外部中断（EXTI）支持多种触发方式，分别是（）。',['上升沿触发方式','下降沿触发方式','双边沿触发方式','高电平触发方式'],'ABC','STM32F1 EXTI支持上升沿、下降沿或两者触发。',42],
  [42,'停止位位于数据帧末尾，为逻辑“1”，通常可取（）。',['1','1.5','2','3'],'ABC','USART常用停止位配置包括1、1.5、2位；3位不支持。',44],
  [43,'串行通信的数据传输模式有（）。',['单工','满工','半双工','全双工'],'ACD','单工、半双工、全双工是三种传输模式；“满工”不是规范术语。',45],
  [44,'STM32串口通信采用TTL电平，由三条线构成为（）。',['TxD（发送）','RxD（接收）','RTS','GND（信号/电源地）'],'ABD','基本串口连接使用发送、接收和公共地。',46],
  [45,'可采用USB转串口芯片实现计算机与单片机之间通信，常用芯片有（）。',['CH350','CH340','PL2303','FT232'],'BCD','CH340、PL2303、FT232是常见USB-UART桥接芯片；CH350主要是PCI串口芯片。',47],
];

const judge = [
  [46,'AHB的含义是高性能总线。','A','AHB为Advanced High-performance Bus。',49],
  [47,'高速内部时钟HSI一般提供72MHz时钟。','B','STM32F103的HSI标称为8MHz，72MHz通常由PLL倍频获得。',50],
  [48,'当下拉电阻接入且输入模式为下拉输入时，若输入信号断开，则引脚输入为高电平。','B','无外部驱动时下拉电阻使输入为低电平。',51],
  [49,'STM32F103ZET6有100个引脚。','A','器件型号中的Z对应100引脚封装。',54],
  [50,'USART_init()是串行口初始化库函数。','A','标准外设库对应函数名为USART_Init()；截图大小写不规范，按函数含义判定。',55],
  [51,'PWM mode 2：CNT<CCRx时输出有效电平；CNT>CCRx时输出无效电平。','B','PWM模式2在CNT<CCR时为无效电平，之后为有效电平。',56],
  [52,'low：通道极性为低电平，也可以说有效电平为低电平，此时高电平就是无效电平。','A','低有效极性下，低电平为有效状态。',57],
  [53,'向上计数模式从初始值0开始加法计数，加到预设值产生一次溢出事件，然后重新从0开始计数。','A','向上计数到ARR后产生更新事件并重新计数。',59],
  [54,'STM32的中断优先级分为抢占优先级和响应优先级两种，设定数字越小代表优先级越低。','B','NVIC优先级数值越小，优先级越高。',60],
  [55,'波特率是每秒钟传送数据的位数，单位为bps，用户可根据需要设定。','A','在二进制串口通信中，一符号携带一位时波特率数值与bps相同；严格讲波特率单位是Bd。',61],
];

const headers = ['题目','A','B','C','D','答案','解析','原题号','来源截图'];
const groupDefs = [
  ['嵌入式单选题.xlsx', '单选题', single.filter(row => ![24,32].includes(row[0]))],
  ['嵌入式多选题.xlsx', '多选题', multiple],
  ['嵌入式判断题.xlsx', '判断题', judge.map(([number,question,answer,explanation,source]) =>
    [number,question,['对','错'],answer,explanation,source])],
];
for (const [fileName, sheetName, rows] of groupDefs) {
  const workbook = Workbook.create();
  const sheet = workbook.worksheets.add(sheetName);
  sheet.showGridLines = false;
  sheet.freezePanes.freezeRows(1);
  const matrix = [headers, ...rows.map(([number, question, choices, answer, explanation, source]) =>
    [question, ...(choices.concat(['','','','']).slice(0,4)), answer.length > 1 ? answer.split('').join(',') : answer, explanation, number, pictures[source]])];
  sheet.getRange(`A1:I${matrix.length}`).values = matrix;
  sheet.getRange(`A1:I${matrix.length}`).format.font = {name:'Arial',size:10,color:'#263238'};
  sheet.getRange('A1:I1').format = {fill:'#1D5B73',font:{name:'Arial',size:10,bold:true,color:'#FFFFFF'},rowHeight:30,verticalAlignment:'center'};
  sheet.getRange(`A2:I${matrix.length}`).format.rowHeight = 68;
  sheet.getRange(`A2:I${matrix.length}`).format.verticalAlignment = 'center';
  sheet.getRange(`A2:I${matrix.length}`).format.wrapText = true;
  for (const [column, width] of Object.entries({A:62,B:37,C:37,D:37,E:37,F:10,G:58,H:10,I:50})) sheet.getRange(`${column}:${column}`).format.columnWidth = width;
  sheet.getRange(`F2:F${matrix.length}`).format.font = {name:'Arial',size:10,bold:true,color:'#166534'};
  sheet.getRange(`H2:H${matrix.length}`).setNumberFormat('0');
  sheet.getRange(`A1:I${matrix.length}`).format.borders = {insideHorizontal:{style:'thin',color:'#DDE5EA'}};
  if (sheetName === '单选题') {
    const review = workbook.worksheets.add('未导入题目');
    review.showGridLines = false;
    review.getRange('A1:H4').values = [
      ['原题号','题目','A','B','C','D','答案','未导入原因'],
      [24,'STM32F103系列单片机不具有PWM功能的是（）。','TIM1','TIM2','TIM3','TIM6','D','与原题15完全重复。'],
      [25,'PSC=79，ARR=99，则定时时间为（）。','1ms','2ms','3ms','4ms','',
        '截图没有给出定时器输入时钟频率。周期=(PSC+1)×(ARR+1)/f_TIM，无法唯一确定答案。'],
      [32,'关于中断嵌套说法正确的是（）。','只要响应优先级不一样就有可能发生中断嵌套','只要抢占式优先级不一样就有可能发生中断嵌套','只有抢占式优先级和响应优先级都不一样才有可能发生中断嵌套','以上说法都不对','B','与原题5完全重复。']
    ];
    review.getRange('A1:H1').format = {fill:'#9A5800',font:{name:'Arial',size:10,bold:true,color:'#FFFFFF'},rowHeight:30};
    review.getRange('A2:H4').format.rowHeight = 64;
    review.getRange('A2:H4').format.wrapText = true;
    for (const [column,width] of Object.entries({A:12,B:58,C:12,D:12,E:12,F:12,G:12,H:70})) review.getRange(`${column}:${column}`).format.columnWidth=width;
  }
  workbook.recalculate();
  const inspected = await workbook.inspect({kind:'table',range:`${sheetName}!A1:I4`,include:'values',tableMaxRows:4,tableMaxCols:9,maxChars:3000});
  console.log(fileName, rows.length, inspected.ndjson);
  const preview = await workbook.render({sheetName,range:'A1:G4',scale:1,format:'png'});
  await fs.writeFile(path.join(outputDir, fileName.replace('.xlsx','.png')),new Uint8Array(await preview.arrayBuffer()));
  const output = await SpreadsheetFile.exportXlsx(workbook);
  await output.save(path.join(outputDir,fileName));
}
