#ifndef _CEGUI_HASHMAP_TEMPLATES_H_
#define _CEGUI_HASHMAP_TEMPLATES_H_
#include <hash_map>
 
#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4786)
#endif
 
using namespace std;
 
namespace CEGUI
{
    /*********************************************************************************************
    Method      : getEntryFromHashMap
    Description : The following template methods provide generic methods, such as adding,
                  searching and deleting entries to/from a HashMap. The HashMap is a hash_map
                  with attributes of type Z as an identifying key and attributes of class T as
                  the object to be stored.
                  This template method is used to search an object in the HashMap, using the id
                  as search key.
    Parameters  : NA
    Returnvalue : NA
    **********************************************************************************************/
    template<typename Z, class T>
    T getEntryFromHashMap (Z id, hash_map<Z, T>* hashMap)
    {
        // Search the entry in the hashMap
        if (hashMap)
        {
            hash_map<Z, T>::iterator hashMapIterator;
            hashMapIterator = hashMap->find (id);
            if (hashMapIterator != hashMap->end())
            {
                // Found it
                return hashMapIterator->second;
            }
        }
 
        return NULL;
    };
 
    /*********************************************************************************************
    Method      : addEntryToHashMap
    Description : This template method is used to add an entry to the hashMap. If an entry
                  with the same key is already present, the stored object is deleted!
    Parameters  : NA
    Returnvalue : NA
    **********************************************************************************************/
    template<typename Z, class T>
    void addEntryToHashMap (Z id, T object, hash_map<Z, T>* hashMap)
    {
        if (hashMap)
        {
            T existingObject = getEntryFromHashMap (id, hashMap);
            if (existingObject == NULL)
            {
                // Not found; insert it
                hashMap->insert (make_pair(id, object));
            }
            else
            {
                if (object != existingObject)
                {
                    // The key exists, but the object differs
                    hashMap->erase (id); // Remove the entry from the hash_map
                    delete existingObject; // Delete the object
                    hashMap->insert (make_pair(id, object));
                }
            }
        }
    };
 
    /*********************************************************************************************
    Method      : eraseEntryFromHashMap
    Description : This template method is used to remove one entry from the hashMap. The entry itself
                  will not be deleted!
    Parameters  : NA
    Returnvalue : NA
    **********************************************************************************************/
    template<typename Z, class T>
    void eraseEntryFromHashMap (Z id, hash_map<Z, T>* hashMap)
    {
        // Remove one entry; it wil not be deleted
        if (hashMap)
        {
            hash_map<Z, T>::iterator hashMapIterator;
            hashMapIterator = hashMap->find (id);
            if (hashMapIterator != hashMap->end())
            {
                // Found it
                hashMap->erase (hashMapIterator);
            }
        }
    };
 
    /*********************************************************************************************
    Method      : deleteEntryFromHashMap
    Description : This template method is used to remove one entry from the hashMap and delete it.
    Parameters  : NA
    Returnvalue : NA
    **********************************************************************************************/
    template<typename Z, class T>
    void deleteEntryFromHashMap (Z id, hash_map<Z, T>* hashMap)
    {
        // Remove one entry; it wil not be deleted
        if (hashMap)
        {
            hash_map<Z, T>::iterator hashMapIterator;
            hashMapIterator = hashMap->find (id);
            if (hashMapIterator != hashMap->end())
            {
                // Found it
                hashMap->erase (hashMapIterator);
                delete hashMapIterator->second;
            }
        }
    };
 
    /*********************************************************************************************
    Method      : deleteAllEntriesFromHashMap
    Description : This template method is used to delete all entries from the hashMap.
    Parameters  : NA
    Returnvalue : NA
    **********************************************************************************************/
    template<typename Z, class T>
    void deleteAllEntriesFromHashMap (hash_map<Z, T>* hashMap)
    {
        // Delete all hashMap entries
        if (hashMap)
        {
            hash_map<Z, T>::iterator hashMapIterator;
            for (hashMapIterator = hashMap->begin(); hashMapIterator != hashMap->end(); hashMapIterator++)
            {
                if (hashMapIterator->second)
                    delete hashMapIterator->second;
            }
            hashMap->clear();
        }
    };
};
#endif