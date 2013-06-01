.. highlight:: php
   :linenothreshold: 2

.. index::
   single: polecenia konsoli; generate:doctrine:form
   single: generowanie formularza

Generowanie nowego formularza opartego na encji Doctrine
--------------------------------------------------------

Stosowanie
~~~~~~~~~~

Polecenie **generate:doctrine:form** generuje podstawową klasę formularza przez
zastosowanie odwzorowania metadanych określonej klasy encji:

.. code-block:: bash
   
   $ php app/console generate:doctrine:form AcmeBlogBundle:Post
   
Wymagane argumenty
~~~~~~~~~~~~~~~~~~

*  ``entity``: Nazwa encji podana w skróconej notacji, zawierająca nazwę pakietu,
   w którym znajduje się encja i nazwę encji. Na przykład: ``AcmeBlogBundle:Post``:
   
   .. code-block:: bash
   
      $ php app/console generate:doctrine:form AcmeBlogBundle:Post
      
