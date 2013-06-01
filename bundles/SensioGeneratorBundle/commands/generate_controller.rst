.. highlight:: php
   :linenothreshold: 2

.. index::
   single: polecenia konsoli; generate:controller
   single: generowanie kontrolera

Generowanie nowego kontrolera
=============================


Stosowanie
----------

Polecenie ``generate:controller`` generuje nowy kontroler, w tym jego akcje, testy,
szablony i trasowanie.

Domyślnie polecenie to jest uruchamiane w trybie interaktywnym i zadaje pytania
w celu ustalenia nazwy pakietu, położenia, formatu konfiguracji i domyślnej struktury:

.. code-block:: bash

    $ php app/console generate:controller

Polecenie to można też uruchomić w trybie nie interaktywnym używając opcji
``--no-interaction`` i nie zapominając przy tym o podaniu wszystkich innych
niezbędnych opcji:

.. code-block:: bash

    $ php app/console generate:controller --no-interaction --controller=AcmeBlogBundle:Post

Dostępne opcje
--------------

* ``--controller``: Nazwa kontrolera podana w skróconej konwencji zawierająca nazwę
  pakietu w którym znajduje się kontroler i nazwę kontrolera.
  Na przykład: ``AcmeBlogBundle:Post`` (tworzy ``PostController`` w pakiecie ``AcmeBlogBundle``):

    .. code-block:: bash

        $ php app/console generate:controller --controller=AcmeBlogBundle:Post

* ``--actions``: Lista akcji do wygenerowania w klasie kontrolera. Ma ona format
  taki jak ``%actionname%:%route%:%template``, gdzie ``:%template%`` jest opcjonalne:

    .. code-block:: bash

        $ php app/console generate:controller --actions="showPostAction:/article/{id} getListAction:/_list-posts/{max}:AcmeBlogBundle:Post:list_posts.html.twig"
        
        # lub
        $ php app/console generate:controller --actions=showPostAction:/article/{id} --actions=getListAction:/_list-posts/{max}:AcmeBlogBundle:Post:list_posts.html.twig

* ``--route-format``: (**annotation**) [values: yml, xml, php or annotation] 
  Opcja ta określa format, jaki ma zostać zastosowany w trasowaniu. Domyślnie
  polecenie stosuje format ``annotation``:

    .. code-block:: bash

        $ php app/console generate:controller --route-format=annotation

* ``--template-format``: (**twig**) [values: twig or php] Opcja ta określa format
  szablonów. Domyślnie polecenie stosuje format ``twig``:

    .. code-block:: bash

        $ php app/console generate:controller --template-format=twig
