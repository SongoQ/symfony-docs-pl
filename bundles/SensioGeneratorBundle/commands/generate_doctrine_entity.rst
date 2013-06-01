.. highlight:: php
   :linenothreshold: 2

.. index::
   single: polecenia konsoli; generate:doctrine:entity
   single: generowanie kawałka encji Doctrine
   
Generowanie nowego kawałka encji Doctrine
-----------------------------------------

Stosowanie
~~~~~~~~~~

Polecenie **generate:doctrine:entity** generuje nowy kawałek encji Doctrine, w tym
definicję odwzorowania i właściwości klasy, metody akcesorów pobierających i ustawiających.

Domyślnie polecenie jest uruchamiane w trybie interaktywnym i zadaje pytania w celu
ustalenia nazwy pakietu, lokalizacji, formatu konfiguracji i domyślnej struktury:

.. code-block:: bash
   
   $ php app/console generate:doctrine:entity
   
Polecenie to można uruchomić w trybie zwykłym (nieinteraktywnym) używając opcję
``--no-interaction``, nie zapominając o przekazaniu wszystkich niezbędnych opcji:

.. code-block:: bash
   
   $ php app/console generate:doctrine:entity --no-interaction --entity=AcmeBlogBundle:Post --fields="title:string(100) body:text" --format=xml

Dostępne opcje
~~~~~~~~~~~~~~

*  ``--entity``: The entity name given as a shortcut notation containing the bundle
   name in which the entity is located and the name of the entity. For example: ``AcmeBlogBundle:Post``:
   
   .. code-block:: bash
      
      $ php app/console generate:doctrine:entity --entity=AcmeBlogBundle:Post
      
*  ``--fields``: The list of fields to generate in the entity class:

   .. code-block:: bash
      
      $ php app/console generate:doctrine:entity --fields="title:string(100) body:text"
      
*  ``--format``: (**annotation**) [wartości: yml, xml, php or annotation]
   This option determines the format to use for the generated configuration files
   like routing. By default, the command uses the annotation format:
   
   .. code-block:: bash
      
      $ php app/console generate:doctrine:entity --format=annotation
      
*  ``--with-repository``: Opcja ta informuje, czy generować związaną klasę EntityRepository
   biblioteki Doctrine:
   
   .. code-block:: bash
      
      $ php app/console generate:doctrine:entity --with-repository
      
   