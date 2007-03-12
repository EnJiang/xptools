#ifndef WED_SELECT_H
#define WED_SELECT_H

#include "WED_Thing.h"
#include "GUI_Broadcaster.h"
#include "ISelection.h"

class	WED_Select : public WED_Thing, public virtual ISelection, public GUI_Broadcaster { 

DECLARE_PERSISTENT(WED_Select)

public:

	// ISelection
	virtual		bool			IsSelected(IUnknown * who) const;

	virtual		void			Select(IUnknown * who);
	virtual		void			Clear (void			 );
	virtual		void			Toggle(IUnknown * who);
	virtual		void			Insert(IUnknown * who);
	virtual		void			Erase (IUnknown * who);

	virtual		void			GetSelectionSet(set<IUnknown *>& sel) const;
	virtual		void			GetSelectionVector(vector<IUnknown *>& sel) const;

	// WED_Persistent
	virtual	void 			ReadFrom(IOReader * reader);
	virtual	void 			WriteTo(IOWriter * writer);
	virtual void			FromDB(sqlite3 * db);
	virtual void			ToDB(sqlite3 * db);
	
private:

	set<int>		mSelected;

};
	
#endif
