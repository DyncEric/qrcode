// QR_Encode.h : CQR_Encode クラス宣言およびインターフェイス定義  类声明和接口定义
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

#if !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
#define AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////
// 定数 不变

// 誤り訂正レベル
// 纠错等级
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// データモード
// 数据模式
#define QR_MODE_NUMERAL		0  // Numeric mode 数字编码
#define QR_MODE_ALPHABET	1  // Alphanumeric mode 字符编码
#define QR_MODE_8BIT		2  // Byte mode, 字节编码，
#define QR_MODE_KANJI		3  // Kanji mode 这是日文编码，也是双字节编码。同样，也                              //可以用于中文编码。日文和汉字的编码会减去一个值。

// バージョン(型番)グループ
// 版本（型号）组
/*我们先说一下二维码一共有40个尺寸。官方叫版本Version。Version 1是21 x 21的矩阵，Version 2是 25 x 25的矩阵，Version 3是29的尺寸，每增加一个version，就会增加4的尺寸，公式是：(V-1)*4 + 21（V是版本号） 最高Version 40，(40-1)*4+21 = 177，所以最高是177 x 177 的正方形*/
#define QR_VRESION_S	0 // 1 ～ 9  代表尺寸
#define QR_VRESION_M	1 // 10 ～ 26
#define QR_VRESION_L	2 // 27 ～ 40

#define MAX_ALLCODEWORD	 3706 // 総コードワード数最大値  总的码字的最大值
#define MAX_DATACODEWORD 2956 // データコードワード最大値(バージョン40-L) 数据码字                             // 的最大值（版本40-L）
#define MAX_CODEBLOCK	  153 // ブロックデータコードワード数最大値(ＲＳコードワードを含む)
							  // 值的块数据的码字的最大数量（包括RS码字）
#define MAX_MODULESIZE	  177 // 一辺モジュール数最大値  最大值一侧的模块数

// ビットマップ描画時マージン  在保证金位绘图
#define QR_MARGIN	4 //MARGIN边缘


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// ＲＳブロック数 块的数目
	int ncAllCodeWord;	// ブロック内コードワード数 在代码字块的数量
	int ncDataCodeWord;	// データコードワード数(コードワード数 - ＲＳコードワード数)
						// 字的数据的代码编号（字码数 - RS码字的数目）

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QRコードバージョン(型番)関連情報  代码版本（型号）的相关信息

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // バージョン(型番)番号(1～40)  版本（型号）号（1-40）
	int ncAllCodeWord; // 総コードワード数  总共的code word数量

	// 以下配列添字は誤り訂正率(0 = L, 1 = M, 2 = Q, 3 = H) 
	// 下面的数组下标纠错率（0= L，1= M2= Q，3= H）
	int ncDataCodeWord[4];	// データコードワード数(総コードワード数 - ＲＳコード                        //  ワード数)
							// 数据代码字的数目（总共的code word数量- RScode word         // 数量）

	int ncAlignPoint;	// アライメントパターン座標数 对齐图案坐标数
	int nAlignPoint[6];	// アライメントパターン中心座標 对齐图案中心的坐标

	RS_BLOCKINFO RS_BlockInfo1[4]; // ＲＳブロック情報(1) 块信息（1）
	RS_BLOCKINFO RS_BlockInfo2[4]; // ＲＳブロック情報(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode クラス  类

class CQR_Encode
{
// 構築/消滅
// 构造/析构
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// 誤り訂正レベル  纠错等级
	int m_nVersion;		// バージョン(型番) 版本（型号）
	BOOL m_bAutoExtent;	// バージョン(型番)自動拡張指定フラグ 
	                    // 版本（型号）的自动指定的扩展标志             
	int m_nMaskingNo;	// マスキングパターン番号 屏蔽图案编号
	/*那些点并不均衡，如果出现大面积的空白或黑块，会告诉我们扫描识别的困难。所以，我们还要做Masking操作（靠，还嫌不复杂）QR的Spec中说了，QR有8个Mask你可以使用，如下所示：其中，各个mask的公式在各个图下面。所谓mask，说白了，就是和上面生成的图做XOR操作。Mask只会和数据区进行XOR，不会影响功能区。（注：选择一个合适的Mask也是有算法的）*/

public:
	int m_nSymbleSize;
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit5:機能モジュール（マスキング対象外）フラグ
	// 功能模块（掩蔽物体之外）的标志
	// bit4:機能モジュール描画データ
	// 第4位：功能模块的绘图数据
	// bit1:エンコードデータ
	// 第1位：编码数据
	// bit0:マスク後エンコード描画データ
	// 位0：拉丝编码后的数据掩盖
	// 20hとの論理和により機能モジュール判定、11hとの論理和により描画（最終的にはBOOL値化）
	//由20H的逻辑和所确定的功能模块，由11h的逻辑和绘制（布尔值最终）

private:
	int m_ncDataCodeWordBit; // データコードワードビット長 数据码字位长度
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // 入力データエンコードエリア 
											 // 输入数据编码区

	int m_ncDataBlock;
	BYTE m_byBlockMode[MAX_DATACODEWORD];
	int m_nBlockLength[MAX_DATACODEWORD];

	int m_ncAllCodeWord; // 総コードワード数(ＲＳ誤り訂正データを含む)
						 //我包括RS纠错数据
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 総コードワード算出エリア总codeword计									 // 算面积
	BYTE m_byRSWork[MAX_CODEBLOCK]; // ＲＳコードワード算出ワーク RS码字计算工作

// データエンコード関連ファンクション
// 数据编码相关的功能
public:
	BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);

private:
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, WORD wData, int ncData);

	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);

	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);

	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

// モジュール配置関連ファンクション
// 模块布局相关的功能
private:
	void FormatModule();

	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
