.. highlight:: php
   :linenothreshold: 2

.. index::
   single: polecenia konsoli; generate:doctrine:crud
   single: generowanie kontrolera CRUD
   
Generowanie kontrolera CRUD opartego na encje Doctrine
------------------------------------------------------

Stosowanie
~~~~~~~~~~

Polecenie **generate:doctrine:crud** generuje podstawowy kontroler dla danej encji
zlokalizowanej w określonym pakiecie. Kontroler ten pozwala na wykonanie pięciu podstawowych
operacji na modelu.

*  Listowanie wszystkich rekordów;
*  Pokazywanie jednego rekordu identyfikowanego przez jego klucz podstawowy;
*  Tworzenie nowego rekordu;
*  Edytowanie istniejącego rekordu;
*  Usuwanie istniejącego rekodu.

By default the command is run in the interactive mode and asks questions to determine
the entity name, the route prefix or whether or not to generate write actions:

.. code-block:: bash
   
   $ php app/console generate:doctrine:crud
   
To deactivate the interactive mode, use the --no-interaction option but don't forget
to pass all needed options:

.. code-block:: bash
   
   $ php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post --format=annotation --with-write --no-interaction

   
Dostępne opcje
~~~~~~~~~~~~~~

*  ``--entity``: The entity name given as a shortcut notation containing the bundle
   name in which the entity is located and the name of the entity. For example:
   ``AcmeBlogBundle:Post``:
   
   .. code-block:: bash
      
      $ php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post

*  ``--route-prefix``: The prefix to use for each route that identifies an action:
      
   .. code-block:: bash
      
      $ php app/console generate:doctrine:crud --route-prefix=acme_post
      

*  ``--with-write``: (**no**) [values: yes|no] Whether or not to generate the new,
   create, edit, update and delete actions:
   
   .. code-block:: bash
      
      $ php app/console generate:doctrine:crud --with-write
      
*  ``--format``: (**annotation**) [values: yml, xml, php or annotation] Determine
   the format to use for the generated configuration files like routing. By default,
   the command uses the annotation format. Choosing the annotation format expects
   the SensioFrameworkExtraBundle is already installed:
   
   .. code-block:: bash
   
      $ php app/console generate:doctrine:crud --format=annotation
      
