#include "stdafx.h"
#include "ImageMgr.h"

IMPLEMENT_SINGLETON(CImageMgr)

CImageMgr::CImageMgr()
{
}

CImageMgr::~CImageMgr()
{
	/* ����� �̹��� ���� */
	for (auto& pair : m_mapImage)
		delete pair.second;
	m_mapImage.clear();

	for (int i = 0; i < FRUIT_END; ++i)
	{
		for (auto& pImg : m_vecCardImage[i])
			delete pImg;

		m_vecCardImage[i].shrink_to_fit();
		m_vecCardImage[i].clear();
	}
}

CImage * CImageMgr::GetImage(const char * pTexName)
{
	auto& iter_find = m_mapImage.find(pTexName);

	if (iter_find == m_mapImage.end()) // ã�� �̹����� ������ null return
		return nullptr;

	return iter_find->second;
}

CImage * CImageMgr::GetCardImage(const int & iFruitID, const int & iFruitCnt)
{
	// ���� ID �� �߸������� null return
	if (iFruitID < 0 || iFruitID >= FRUIT_END)
		return nullptr;
	// ���� ������ �߸������� null return
	if (iFruitCnt < 0 || iFruitCnt > 5)
		return nullptr;

	return m_vecCardImage[iFruitID][iFruitCnt - 1];
}

void CImageMgr::Initialize()
{
	Load_Image();
	Load_CardImage("../../Image/Card/");

	AfxMessageBox("�̹��� �ε� ����");
}

void CImageMgr::Load_Image()
{
	// ���� ���� �˻� ����
	char szFilePath[MAX_PATH] = "../../Image/";
	strcat_s(szFilePath, "\\*.*");

	CFileFind find;
	BOOL bContinue = find.FindFile(szFilePath);

	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			continue;
		else
		{
			if (find.IsSystem())
				continue; // �ý��� ������ ��� �Ѿ

			CImage* pImage = new CImage;
			pImage->Load(find.GetFilePath());

			CString strFileName = find.GetFileTitle(); // ���ϸ� ������ ����
			m_mapImage.emplace(strFileName, pImage);
		}
	}
}

void CImageMgr::Load_CardImage(const char * pPath)
{
	// ���� ���� �˻�
	char szFilePath[MAX_PATH] = "";
	strcpy_s(szFilePath, pPath);
	strcat_s(szFilePath, "\\*.*");

	CFileFind find;
	BOOL bContinue = find.FindFile(szFilePath);
	
	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			Load_CardImage(find.GetFilePath()); // ������ ��� ���ȣ��
		else
		{
			if(find.IsSystem())
				continue; // �ý��� ������ ��� �Ѿ
			
			char szFileName[MAX_PATH] = "";
			strcpy_s(szFileName, find.GetFilePath());

			PathRemoveFileSpec(szFileName); // ���ϸ�� Ȯ���� ����
			CString strImgName = PathFindFileName(szFileName); // ���� ������

			if (strImgName == "Back")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_BACK].push_back(pImage);
			}
			else if (strImgName == "Cherry")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_CHEERY].push_back(pImage);
			}
			else if (strImgName = "Pear")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_PEAR].push_back(pImage);
			}
		}
	}
}