/*****************************************************************
*�� �� ��: LoadTxnfile.c
*��	ע:
******************************************************************/
#include "Ibp.h"
#include "pos_stlm.h"
#include "DbsApi.h"
#include "load.h"

typedef struct _CUPSKEYFUN
{
	char	szCupsNo[20];
	int		(*pfCupsKeyFun)(TTxnSucc *pstTxn);
	char	szCupsDes[100];
}KeyFunc;

int		nIboxLocalLoad(TTxnSucc *pszTxnSucc);
int		nCilkLocalLoad(TTxnSucc *pszTxnSucc);
int		nSpdbLocalLoad(TTxnSucc *pszTxnSucc);
int		nTftpLocalLoad(TTxnSucc *pszTxnSucc);
int		nHxbpLocalLoad(TTxnSucc *pszTxnSucc);
int		nBillLocalLoad(TTxnSucc *pszTxnSucc);
int		nUmpyLocalLoad(TTxnSucc *pszTxnSucc);
int		nHkrtLocalLoad(TTxnSucc *pszTxnSucc);
int		nCofpLocalLoad(TTxnSucc *pszTxnSucc);
int		nSqpyLocalLoad(TTxnSucc *pszTxnSucc);
int		nCupsLocalLoad(TTxnSucc *pszTxnSucc);
int		nCntpLocalLoad(TTxnSucc *pszTxnSucc);
int		nCmspLocalLoad(TTxnSucc *pszTxnSucc);
int		nScupLocalLoad(TTxnSucc *pszTxnSucc);
int		nAlpyLocalLoad(TTxnSucc *pszTxnSucc);
int		nJzdkLocalLoad(TTxnSucc *pszTxnSucc);
int		nWchtLocalLoad(TTxnSucc *pszTxnSucc);
int		nCpnrLocalLoad(TTxnSucc *pszTxnSucc);
int		nBjysLocalLoad(TTxnSucc *pszTxnSucc);
int		nHkubLocalLoad(TTxnSucc *pszTxnSucc);
int		nWfjpLocalLoad(TTxnSucc *pszTxnSucc);
int		nRyxpLocalLoad(TTxnSucc *pszTxnSucc);
int		nYsepLocalLoad(TTxnSucc *pszTxnSucc);
int		nTlpyLocalLoad(TTxnSucc *pszTxnSucc); 
int		nHdsyLocalLoad(TTxnSucc *pszTxnSucc); 
int		nCdifLocalLoad(TTxnSucc *pszTxnSucc); 

KeyFunc g_KeyFun[] = {
		{"ibox",  nIboxLocalLoad, "���ö�������Keyֵ����"},
		{"cilk",  nCilkLocalLoad, "��֯Ѷ������KEYֵ"},
		{"spdb",  nSpdbLocalLoad, "��֯�ַ�����KEYֵ"},
		{"tftp",  nTftpLocalLoad, "��֯�ڸ�ͨ����KEYֵ"},
		{"hxbp",  nHxbpLocalLoad, "��֯���ζ���KEYֵ"},
		{"hxnp",  nHxbpLocalLoad, "��֯������ƽ̨����KEYֵ"},
		{"bill",  nBillLocalLoad, "��֯��Ǯ����KEYֵ"},
		{"umpy",  nUmpyLocalLoad, "��֯�������ƶ���KEYֵ"},
		{"hkrt",  nHkrtLocalLoad, "��֯������ͨ����KEYֵ"},
		{"cofp",  nCofpLocalLoad, "��֯����ͨ����KEYֵ"},
		{"sqpy",  nSqpyLocalLoad, "��֯����001����KEYֵ"},
		{"cups",  nCupsLocalLoad, "��֯����������KEYֵ"},
		{"cntp",  nCntpLocalLoad, "��֯����ͨ����KEYֵ"},
		{"cntn",  nCntpLocalLoad, "��֯����ͨ��ƽ̨����KEYֵ"},
		{"cmsp",  nCmspLocalLoad, "��֯cmsp����KEYֵ"},
		{"scup",  nScupLocalLoad, "��֯scup����KEYֵ"},
		{"alpy",  nAlpyLocalLoad, "��֧֯��������KEYֵ"},
		{"jzdk",  nJzdkLocalLoad, "��֯��������KEYֵ"},
		{"wcht",  nWchtLocalLoad, "��֯΢�Ŷ���KEYֵ"},
		{"cpnr",  nCpnrLocalLoad, "��֯�㸶����KEYֵ"},
		{"xcpy",  nCpnrLocalLoad, "��֯�㸶����KEYֵ"},
		{"xcpa",  nCpnrLocalLoad, "��֯�㸶����KEYֵ"},
		{"bjys",  nBjysLocalLoad, "��֯�������̶���KEYֵ"},
		{"bjnp",  nBjysLocalLoad, "��֯�������̶���KEYֵ"},
		{"hkub",  nHkubLocalLoad, "��֯���̶���KEYֵ"},
		{"wfjp",  nWfjpLocalLoad, "��֯������KEYֵ"},
		{"ryxp",  nRyxpLocalLoad, "��֯�����Ŷ���KEYֵ"},
		{"ysep",  nYsepLocalLoad, "��֯��ʢ֧������KEYֵ"},
		{"ysea",  nYsepLocalLoad, "��֯��ʢ֧������KEYֵ"},
		{"tlpy",  nTlpyLocalLoad, "��֯ͨ��֧��KEYֵ"},
		{"hdsy",  nHdsyLocalLoad, "��֯�����ҶKEYֵ"},
		{"hdsx",  nHdsyLocalLoad, "��֯�����ҶKEYֵ"},
		{"wfjn",  nWfjpLocalLoad, "��֯��������ƽ̨KEYֵ"},
		{"wfja",  nWfjpLocalLoad, "��֯����������ͨ��KEYֵ"},
		{"cdif",  nCdifLocalLoad, "��֯����ƽ̨KEYֵ"},
		{"cdia",  nCdifLocalLoad, "��֯��������ƽ̨KEYֵ"}
};

/************************************************************************************/
/*   ����  �� int nGetCupKey														*/
/*   ����  �� ���ö�Ӧ����������Keyֵ												*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nGetCupKey(char *pszCupsNo, TTxnSucc *pstTxn)
{
	int		 i = 0, lFlag = 0;

	if(TRADE_TXN_INIT != pstTxn->result_flag[0])
		return RC_SUCC;

	for(i = 0; i < sizeof(g_KeyFun)/sizeof(KeyFunc); i ++)
	{
		if(!memcmp(g_KeyFun[i].szCupsNo, pszCupsNo, strlen(pszCupsNo)))
			return g_KeyFun[i].pfCupsKeyFun(pstTxn);
	}

	HtLog(ERR_PARAM, "����(%s)����CUP_KEYδ�������ɷ���, Ĭ�Ϲ�����������(%s)��", 
		pstTxn->cups_no, IBP_COM_CUPSNO);

	return nIboxLocalLoad(pstTxn);
}

/************************************************************************************/
/*   ����  �� int nIboxLocalLoad 													*/
/*   ����  �� ��֯���ö���CUP_KEY													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nIboxLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	if(!strlen(pstTxn->tx_date) || !strlen(pstTxn->fd13) || 
		!strlen(pstTxn->cups_no) || !strlen(pstTxn->fd11))
	{
		
		HtLog(ERR_PARAM, "��������(%s)(%s)(%s)!", pstTxn->fd13, 
			pstTxn->fd11, pstTxn->cups_no);
		return RC_FAIL;
	}

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no, 
		szDate, pstTxn->fd11); 

	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCntpLocalLoad													*/
/*   ����  �� ��֯����ͨ����CUP_KEY													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nCntpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no, 
		szDate, pstTxn->fd11); 

	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCntpLocalLoad													*/
/*   ����  �� ��֯Ѷ������CUP_KEY												   	*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nCilkLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no, 
		szDate, pstTxn->fd11); 
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nSpdbLocalLoad													*/
/*   ����  �� ��֯�ַ�����CUP_KEY												 	*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nSpdbLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no, 
		szDate, pstTxn->fd11); 
	return RC_SUCC;
}
/************************************************************************************/
/*   ����  �� int nHxbpLocalLoad													*/
/*   ����  �� ��֯����CUP_KEY														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nTftpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	} 
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no, 
		szDate, pstTxn->fd11); 
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nHxbpLocalLoad													*/
/*   ����  �� ��֯����CUP_KEY														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nHxbpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nHxbpLocalLoad													*/
/*   ����  �� ��֯����CUP_KEY														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nHxnpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nSqpyLocalLoad													*/
/*   ����  �� ��֯����������KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nSqpyLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCupsLocalLoad													*/
/*   ����  �� ��֯����������KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nCupsLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nBillLocalLoad													*/
/*   ����  �� ��֯��Ǯ����KEYֵ														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nBillLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nUmpyLocalLoad													*/
/*   ����  �� ��֯�������ƶ���KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nUmpyLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nHkrtLocalLoad													*/
/*   ����  �� ��֯������ͨ����KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nHkrtLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCofpLocalLoad													*/
/*   ����  �� ��֯����ͨ����KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nCofpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCpnrLocalLoad													*/
/*   ����  �� ��֯����ͨ����KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nCpnrLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nXcpyLocalLoad													*/
/*   ����  �� ��֯��巶���KEYֵ												   		*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nXcpyLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nBjysLocalLoad													*/
/*   ����  �� ��֯�������̶���KEYֵ												 	*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nBjysLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

//	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
//		szDate, pstTxn->fd11);
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key),"%s", pstTxn->fd62);

	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nHkubLocalLoad													*/
/*   ����  �� ��������cup_key����													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nHkubLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCdifLocalLoad													*/
/*   ����  �� ����cup_key����														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nCdifLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s%s%s", pstTxn->cups_no,
		szDate, pstTxn->fd41, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nWfjpLocalLoad													*/
/*   ����  �� ������cup_key����														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nWfjpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nRyxpLocalLoad													*/
/*   ����  �� ��֯�����Ŷ���KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nRyxpLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nYsepLocalLoad													*/
/*   ����  �� ��֯��ʢ֧������KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nYsepLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nTlpyLocalLoad													*/
/*   ����  �� ��֯ͨ��KEY															*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nTlpyLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nHdsyLocalLoad													*/
/*   ����  �� ��֯�����ҵ���ʱ�ʾ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nHdsyLocalLoad(TTxnSucc *pstTxn)
{
	char	szDate[10];
	int		nYear;

	memset(szDate, 0, sizeof(szDate));
	memcpy(szDate, pstTxn->tx_date, 4);	 //  �������
	nYear = atoi(szDate);

	// ��������ʵ��������Ϊ׼
	if(0 == memcmp(pstTxn->tx_date + 4, "1231", 4) && 0 == memcmp(pstTxn->fd13, "0101", 4))
	{
		nYear ++;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else if(0 == memcmp(pstTxn->tx_date + 4, "0101", 4) && 0 == memcmp(pstTxn->fd13, "1231", 4))
	{
		nYear --;
		snprintf(szDate, sizeof(szDate), "%d%s", nYear, pstTxn->fd13);
	}
	else
		memcpy(szDate + 4, pstTxn->fd13, 4);

	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s000000%s", pstTxn->cups_no,
		szDate, pstTxn->fd11);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nJzdkLocalLoad													*/
/*   ����  �� ��֯��������KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nJzdkLocalLoad(TTxnSucc *pstTxn)
{
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s", pstTxn->cups_no,
		pstTxn->order_no);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nAlpyLocalLoad													*/
/*   ����  �� ��֯����ͨ����KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nAlpyLocalLoad(TTxnSucc *pstTxn)
{
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s", pstTxn->cups_no,
		pstTxn->order_no);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nWchtLocalLoad													*/
/*   ����  �� ��֯΢��֧������KEYֵ													*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int		nWchtLocalLoad(TTxnSucc *pstTxn)
{
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s", pstTxn->cups_no,
		pstTxn->order_no);
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nCmspLocalLoad													*/
/*   ����  �� ��֯����CUP_KEY														*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nCmspLocalLoad(TTxnSucc *pstTxn)
{
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s%s", pstTxn->cups_no, 
		pstTxn->tx_date, pstTxn->fd37); 
	return RC_SUCC;
}

/************************************************************************************/
/*   ����  �� int nScupLocalLoad													*/
/*   ����  �� ��֯CUP_KEY															*/
/*  ����ֵ �� RC_SUCC	--  �ɹ�													*/
/*			  RC_FAIL	--  ʧ��													*/
/************************************************************************************/
int nScupLocalLoad(TTxnSucc *pstTxn)
{
	snprintf(pstTxn->cup_key, sizeof(pstTxn->cup_key), "%s%s%s", pstTxn->cups_no, 
		pstTxn->tx_date, pstTxn->fd37); 
	return RC_SUCC;
}