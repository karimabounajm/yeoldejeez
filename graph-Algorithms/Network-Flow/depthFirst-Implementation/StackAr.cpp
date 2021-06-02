        #include "StackAr.h"

        /**
         * Construct the stack.
         */
        template <class Object>
        StackAr<Object>::StackAr(int capacity): theArray( new Object[capacity] )  
        {
            topOfStack = -1;
        }

        /**
         * Test if the stack is logically empty.
         * Return true if empty, false otherwise.
         */
        template <class Object>
        bool StackAr<Object>::isEmpty( ) const
        {
            return topOfStack == -1;
        }

        /**
         * Make the stack logically empty.
         */
        template <class Object>
        void StackAr<Object>::makeEmpty( )
        {
            topOfStack = -1;
        }

        /**
         * Get the most recently inserted item in the stack.
         * Does not alter the stack.
         * Return the most recently inserted item in the stack.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        const Object & StackAr<Object>::top( ) const
        {
            if( isEmpty( ) )
                throw Underflow( );
            return theArray[ topOfStack ];
        }

        /**
         * Remove the most recently inserted item from the stack.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        void StackAr<Object>::pop( )
        {
            if( isEmpty( ) )
                throw new Underflow( );
            topOfStack--;
        }

        /**
         * Insert x into the stack, if not already full.
         * Exception Overflow if stack is already full.
         */
        template <class Object>
        void StackAr<Object>::push( const Object & x )
        {
            theArray[ ++topOfStack ] = x;
        }

        /**
         * Return and remove most recently inserted item from the stack.
         * Return most recently inserted item.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        Object StackAr<Object>::topAndPop( )
        {
            if( isEmpty( ) )
                throw Underflow( );
            return theArray[ topOfStack-- ];
        }
