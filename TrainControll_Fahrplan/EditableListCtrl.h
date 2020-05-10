#pragma once

class CEditableListCtrl : public CListCtrl
{
public:
	int GetRowFromPoint( CPoint &point, int *col ) const;
	CEdit* EditSubLabel( int nItem, int nCol );

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDown(UINT nFlags, CPoint point, int* Col );
	void OnKeydown(int Zeile, int Spalte);
};