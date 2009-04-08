package gwt.cassowary.client.EDU.Washington.grad.gjb;

import java.util.*;

public class Hashtable<K, V> extends HashMap<K, V> {
   private static final long serialVersionUID = 1421746759512286392L;

   public Hashtable() {
      super();
   }

   public Hashtable(int initialCapacity) {
      super(initialCapacity);
   }

   public Hashtable(int initialCapacity, float loadFactor) {
      super(initialCapacity, loadFactor);
   }

   private class EnumerationElements<M, N> implements Enumeration<N> {
      Hashtable<M, N> hashtable;
      Enumeration<M>  keys;

      EnumerationElements(Hashtable<M, N> hashtable) {
         this.hashtable = hashtable;
         this.keys = this.hashtable.keys();
      }

      public boolean hasMoreElements() {
         return keys.hasMoreElements();
      }

      public N nextElement() {
         return hashtable.get(keys.nextElement());
      }
   }

   private class EnumerationKeys<M> implements Enumeration<M> {
      private M[] keys  = null;
      private int index = 0;

      EnumerationKeys(M[] keys) {
         this.keys = keys;
      }

      public boolean hasMoreElements() {
         return index < keys.length;
      }

      public M nextElement() {
         return keys[index++];
      }
   }

   public Enumeration<V> elements() {
      return new EnumerationElements<K, V>(this);
   }

   @SuppressWarnings("unchecked")
   public Enumeration<K> keys() {
      return new EnumerationKeys<K>((K[]) this.keySet().toArray());
   }
}
