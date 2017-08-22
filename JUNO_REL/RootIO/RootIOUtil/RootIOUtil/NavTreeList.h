#ifndef ROOTIOUTIL_NAVTREELIST_H
#define ROOTIOUTIL_NAVTREELIST_H

#include <vector>
#include <string>

class TTree;

class NavTreeHandle {

public:

    NavTreeHandle(int fileid, const std::string& filename);
    ~NavTreeHandle();

    const std::string& filename() { return m_filename; }

    /// Open TFile which owns this tree, return false if fail.
    bool open();

    /// Stream no longer visit this tree
    bool leave();

    /// Return current entry, -1 if no entry yet set
    int entry();

    /// Set entry (but no GetEntry()), return false if illegal value
    bool setEntry(int entry);

    /// Read in current entry to the object pointed to by the pointer
    /// at the given address addr, return false if entry not properly set
    bool read(void*& addr);

    /// Set address of pointer to object to copy branch name into on read().
    bool setAddr(void*& addr);

    /// Return total number of entries
    int entries();

    /// Advance entry number by given number of steps, return false if too far
    bool next(int steps = 1);

    /// Go to first entry
    bool beginning();

    /// Go to last entry
    bool ending();

    /// Retard entry number by given number of steps, return false if too far
    bool prev(int steps = 1);

    bool opened();

    /// For logging
    const std::string& objName() { return m_name; }

  private:
    TTree* m_tree;
    int m_fileID;
    int m_entry, m_entries;
    void* m_addr;
    std::string m_filename, m_name;
    bool m_opened;
};

class NavTreeList : public std::vector<NavTreeHandle*> {
  public:
    NavTreeList();

    ~NavTreeList();

    /// The index of current tree
    int index();

    /// Access the current tree
    NavTreeHandle* current();

    /// Advance one tree
    bool next();

    /// Retard one tree
    bool prev();

    /// Go to last tree
    bool last();

    /// Go to first tree
    bool first();

    /// Go to the tree at given index
    bool jump(int index);

    /// Total number of entries of all trees upto but not including
    /// file at given index.  Returns -1 on error
    int entriesBefore(int index);

    /// for log
    const std::string& name() { return m_name; }

  private:
    int m_index;
    std::string m_name;
};

#endif
