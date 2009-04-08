// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Set
// Encapsulate a mathematical "Set" ADT using java's
// hash table.  Just a convenience wrapper of the java.util.Hashtable class.

package gwt.cassowary.client.EDU.Washington.grad.gjb;

import java.util.*;

class Set extends HashSet<Object> {
   private static final long serialVersionUID = 754973032030234779L;

   public Set() {
      super();
   }

   public Set(int i) {
      super(i);
   }

   public boolean containsKey(Object e) {
      return contains(e);
   }

   public boolean insert(Object e) {
      return add(e);
   }

   private class EnumerationElements<N> implements Enumeration<N> {
      Object[] elements;
      int      index;

      EnumerationElements(Set hashSet) {
         elements = hashSet.toArray();
         index = 0;
      }

      public boolean hasMoreElements() {
         return index < elements.length;
      }

      @SuppressWarnings("unchecked")
      public N nextElement() {
         return (N) elements[index++];
      }
   }

   public Enumeration<Object> elements() {
      return new EnumerationElements<Object>(this);
   }
}
