Przyczynianie się do rozwoju Dokumentacji
=========================================

Dokumentacja jest tak samo ważna jak kod. Wywodzą się z tych samych zasad:
DRY, testy, łatwość utrzymania, rozszerzalność, optymalizacja i refaktoryzacja,
to tylko kilka z nich. Oczywiście, dokumentacja posiada błędy, literówki, ciężkie
do przeczytania tutoriale, i tak dalej.

Wkład
-----

Zanim zaczniesz tworzyć dokumentację, zapoznaj się z stosowanym w dokumentacji
:doc:`językiem znaczników<format>`.

Dokumentacja Symfony2 jest hostowana na GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Jeśli chcesz wysłać poprawkę, wykonaj `fork`_ na oficjalnym repozytorium na GitHub
oraz sklonuj swojego forka:

.. code-block:: bash

    $ git clone git://github.com/YOURUSERNAME/symfony-docs.git

Następnie, stwórz dedykowanego brancha dla swoich zmian:

.. code-block:: bash

    $ git checkout -b improving_foo_and_bar

Możesz tworzyć swoje zmiany bezpośrednio w tym branchu oraz je commitować.
Gdy skończysz, możesz wykonać push brancha na *twój* fork na GitHub oraz
wykonać pull request.
Pull request zostanie wykonany pomiędzy Twoim branchem ``improving_foo_and_bar``
oraz branchem ``master`` z repozytorium ``symfony-docs``.

.. image:: /images/docs-pull-request.png
   :align: center

GitHub opisuje temat `pull requests`_ w szczegółach.

.. note::

    Dokumentacja Symfony2 jest na licencji Creative Commons
    Attribution-Share Alike 3.0 Unported :doc:`License <license>`.

Raportowanie Błędu
------------------

Najprostszym wkładem w rozwój jest zgłaszanie błędów: literówki,
błędu gramatycznego, błędu w przykładowym kodzie, brakującego wyjaśnienia,
i tak dalej.

Kroki:

* Wyślij błąd do bug trackera;

* *(opcjonalnie)* Wyślij poprawkę.

Tłumaczenie
-----------

Przeczytaj dedykowany :doc:`dokument <translations>`.

.. _`fork`: http://help.github.com/fork-a-repo/
.. _`pull requests`: http://help.github.com/pull-requests/
