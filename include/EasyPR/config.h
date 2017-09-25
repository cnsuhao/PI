#ifndef EASYPR_CONFIG_H_
#define EASYPR_CONFIG_H_

#define CV_VERSION_THREE_TWO
#include "opencv2/opencv.hpp"
namespace easypr
{

	enum Color { BLUE, YELLOW, WHITE, UNKNOWN };	// 颜色类型

	enum LocateType { SOBEL, COLOR, CMSER, OTHER }; // 定位方式

	enum CharSearchDirection { LEFT, RIGHT };		// 字符搜索方向

	// PR运用场景模式
	enum
	{
		PR_MODE_UNCONSTRAINED,	// 无约束
		PR_MODE_CAMERPOHNE,		// 相机
		PR_MODE_PARKING,		// 停车场
		PR_MODE_HIGHWAY			// 高速公路
	};

	enum
	{
		PR_DETECT_SOBEL = 0x01,  /**Sobel detect type, using twice Sobel  */
		PR_DETECT_COLOR = 0x02,  /**Color detect type   */
		PR_DETECT_CMSER = 0x04,  /**Character detect type, using mser  */
	};

	static const char* kDefaultSvmFile = "svm_hist.xml";
	static const char* kLBPSvmFile = "svm_lbp.xml";
	static const char* kHistSvmFile = "svm_hist.xml";

	static const char* kDefaultAnnFile = "ann.xml";
	static const char* kChineseAnnFile = "ann_chinese.xml";
	static const char* kGrayAnnFile = "annCh.xml";

	//This is important to for key transform to chinese
	static const char* kChineseMappingFile = "province_mapping";

	typedef enum
	{
		kForward = 1, // correspond to "has plate"
		kInverse = 0  // correspond to "no plate"
	} SvmLabel;

	static const int	DEFAULT_DEBUG = 0;					// 是否默认开启调试模式（显示中间图像）
	static const int	DEFAULT_WINDOW_WIDTH = 1000;		// 默认显示窗口宽
	static const int	DEFAULT_WINDOW_HEIGHT = 800;		// 默认显示窗口高
	static const int	HORIZONTAL = 1;						// 获取图像水平方向特征的标志
	static const int	VERTICAL = 0;						// 获取图像垂直方向特征的标志

	// 车牌定位部分常量
	static const int	DEFAULT_GAUSSIANBLUR_SIZE = 5;		// 默认高斯模糊模板大小
	static const int	SOBEL_DDEPTH = CV_16S;				// sobel滤波输出图像的数据格式
	static const int	SOBEL_SCALE = 1;					// 计算导数时，默认的缩放因子
	static const int	SOBEL_DELTA = 0;					// 加在计算结果上的常量，默认为0
	static const int	SOBEL_X_WEIGHT = 1;					// Sobel X方向滤波图像的叠加权重
	static const int	SOBEL_Y_WEIGHT = 0;					// Sobel Y方向滤波图像的叠加权重
	static const int	DEFAULT_MORPH_SIZE_WIDTH = 17;		// 默认形态学模板的宽
	static const int	DEFAULT_MORPH_SIZE_HEIGHT = 3;		// 默认形态学模板的高

	static const int	DEFAULT_PLATE_DEPTH = CV_8UC3;		// 默认车牌图像的数据格式
	static const int	DEFAULT_PLATE_WIDTH = 136;			// 默认车牌尺寸宽
	static const int	DEFAULT_PLATEHEIGHT = 36;			// 默认车牌尺寸高
	static const int	DEFAULT_VERIFY_MIN = 1;				// 默认车牌区域最小面积系数
	static const int	DEFAULT_VERIFY_MAX = 24;			// 默认车牌区域最大面积系数
	static const int	DEFAULT_ANGLE = 60;					// 默认的用于车牌判断的倾斜角度
	static const float	DEFAULT_ASPECT = 3.75f;				// 默认的车牌宽高比
	static const float	DEFAULT_ERROR = 0.9f;				// 默认的车牌宽高比偏差率
	
	static const int	DEFAULT_JUMP_THRESHOLD = 7;			// 默认柳钉所在行像素跳跃次数阈值为7
	static const int	DEFAULT_LIUDING_SIZE = 7;			// 默认柳钉大小为7
	static const int	DEFAULT_MAT_WIDTH = 136;			// 默认车牌宽度为136
	static const int	DEFAULT_COLORTHRESHOLD = 150;		// 默认颜色阈值为150

	static const float	DEFAULT_SVM_PERCENTAGE = 0.7f;		//  

	// 字符识别部分常量
	static const int	CHAR_SIZE = 20;						// 默认字符图像大小
	static const int	DEFAULT_CHARACTER_INPUT = 120;		// 默认字符鉴别ann输入层节点数
	static const int	DEFAULT_CHARACTER_HIDDEN = 40;		// 默认字符鉴别ann中间层节点数
	static const int	DEFAULT_CHINESE_INPUT = 440;		// 默认中文字符鉴别ann输入层节点数
	static const int	DEFAULT_CHINESE_HIDDEN = 64;		// 默认中文字符鉴别ann中间层节点数
	
	static const int	DEFAULT_CHINESE_CHAR_SIZE = 20;		// 默认中文字符大小为20
	static const int	DEFAULT_LETTER_AND_NUM_SIZE = 10;	// 默认字幕和数字大小为10				

	static const int	LETTERS_AND_NUMS_NUMBER = 34;		// 数字和字母的个数
	static const int	CHINESE_NUMBER = 31;				// 中文字符个数
	static const int	ALL_CHARS_NUMBER = 65;				// 所有字符个数

	static const int	DEFAULT_GRAY_CHAR_WIDTH = 20;		// 默认灰度字符图像宽度
	static const int	DEFAULT_GRAY_CHAR_HEIGHT = 32;		// 默认灰度字符图像高度
	static const int	DEFAULT_CHAR_LBP_GRID_X = 4;		// 默认BP-ann的网格x宽度
	static const int	DEFAULT_CHAR_LBP_GRID_Y = 4;		// 默认BP-ann的网格y宽度
	static const int	DEFAULT_CHAR_LBP_PATTERNS = 16;		// 默认BP-ann的模式数

	static const int	CHARS_COUNT_IN_PLATE = 7;			// 车牌中的字符数，一般为7
	static const int	SYMBOLS_COUNT_IN_CHINESE_PLATE = 6;	// 在中文车牌中的符号数
	static const float	PLATE_MAX_SYMBOLS_COUNT = 7.5f;		// 车牌整个宽度占的字符数，默认约等于7.5个
	static const int	SPECIAL_CHAR_INDEX = 2;				// 特殊字符所在索引

	// 车牌中的字符表：数字+字母+汉字
	static const char *charsTable[] =
	{
	  "0", "1", "2",
	  "3", "4", "5",
	  "6", "7", "8",
	  "9",
		/*  10  */
		"A", "B", "C",
		"D", "E", "F",
		"G", "H", /* {"I", "I"} */
		"J", "K", "L",
		"M", "N", /* {"O", "O"} */
		"P", "Q", "R",
		"S", "T", "U",
		"V", "W", "X",
		"Y", "Z",
		/*  24  */
		"zh_cuan" , "zh_e"    , "zh_gan"  ,
		"zh_gan1" , "zh_gui"  , "zh_gui1" ,
		"zh_hei"  , "zh_hu"   , "zh_ji"   ,
		"zh_jin"  , "zh_jing" , "zh_jl"   ,
		"zh_liao" , "zh_lu"   , "zh_meng" ,
		"zh_min"  , "zh_ning" , "zh_qing" ,
		"zh_qiong", "zh_shan" , "zh_su"   ,
		"zh_sx"   , "zh_wan"  , "zh_xiang",
		"zh_xin"  , "zh_yu"   , "zh_yu1"  ,
		"zh_yue"  , "zh_yun"  , "zh_zang" ,
		"zh_zhe"
		/*  31  */
	};

	// Disable the copy and assignment operator for this class.
#define DISABLE_ASSIGN_AND_COPY(className) \
private:\
  className& operator=(const className&); \
  className(const className&)

// Display the image.
#define SHOW_IMAGE(imgName, debug) \
  if (debug) \
  { \
    namedWindow("imgName", WINDOW_AUTOSIZE); \
    moveWindow("imgName", 500, 500); \
    imshow("imgName", imgName); \
    waitKey(0); \
    destroyWindow("imgName"); \
  }

// Load model. compatitable withe 3.0, 3.1 and 3.2
#ifdef CV_VERSION_THREE_TWO
#define LOAD_SVM_MODEL(model, path) \
    model = ml::SVM::load(path);
#define LOAD_ANN_MODEL(model, path) \
    model = ml::ANN_MLP::load(path);
#else
#define LOAD_SVM_MODEL(model, path) \
    model = ml::SVM::load<ml::SVM>(path);
#define LOAD_ANN_MODEL(model, path) \
    model = ml::ANN_MLP::load<ml::ANN_MLP>(path);
#endif

}

#endif // EASYPR_CONFIG_H_