Zgłoszenie Poprawki (Patch)
===========================

Poprawki są najlepszym sposobem naprawienia błędu lub zaproponowania ulepszenia
dla Symfony2.

Ustawienia początkowe
---------------------

Przed rozpoczęciem pracy z Symfony2, skonfiguruj przyjazne środowisko z poniższym
oprogramowaniem:

* Git;

* PHP w wersji 5.3.2 lub wyższej;

* PHPUnit 3.5.11 lub wyższe.

Ustaw swoją nazwę użytkownika oraz adres e-mail:

.. code-block:: bash

    $ git config --global user.name "Your Name"
    $ git config --global user.email you@example.com

.. tip::

    Jeśli dopiero zacząłeś używać Git, zalecamy zapoznanie się z doskonałą a
    zarazem darmową książką `ProGit`_.

Pobierz kod źródłowy Symfony2:

* Utwórz konto na `GitHub`_ oraz się zaloguj;

* Zforkuj `repozytorium Symfony2`_ (kliknij na przycisk "Fork");

* Po zakończeniu "hardcore forking action", sklonuj swojego forka lokalnie
  (zostanie utworzony katalog `symfony`):

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Dodaj repozytorium "upstream" jako repozytorium ``zdalne``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Teraz gdy Symfony2 zostało zainstalowane, sprawdź czy wszystkie unit testy przechodzą
na Twoim środowisku - zostało to wyjaśnione w dedykowanym :doc:`dokumencie <tests>`.

Praca z Poprawkami (Patch)
--------------------------

Za każdym razem gdy chcesz pracować nad poprawką błędu lub też ulepszeniem,
musisz utworzyć branch tematyczny.

Branch ten powinien bazować na branchu `master` jeśli chcesz dodać nową funkcjonalność.
Ale jeśli chcesz naprawić błąd, użyj starszej ale nadal wspieranej wersji Symfony
w której błąd występuje (np. `2.0`).

Utwórz branch tematyczny używając następującej komendy:

.. code-block:: bash

    $ git checkout -b BRANCH_NAME master

.. tip::

    Użyj opisowej nazwy dla swojego brancha (`ticket_XXX` gdzie `XXX` jest
    numerem zgłoszenia, jest to dobra konwencja do zgłaszania poprawek błędów).

Powyższa komenda automatycznie przełączy kod do nowo utworzonego brancha
(sprawdź nad którym branchem pracujesz używając `git branch`).

Pracuj nad kodem jak chcesz, oraz commituj ile chcesz; ale miej na uwadze:

* Stosuj się do :doc:`standardów <standards>` kodowania (użyj `git diff --check` aby
  sprawdzić czy nie masz zbędnych spacji (trailing spaces));

* Dodaj unit test aby udowodnić że błąd został poprawiony lub że nowa funkcjonalność
  działa;

* Rób atomowe oraz logicznie podzielone commity (wykorzystaj moc `git rebase`
  aby mieć czystą i logiczną historię);

* Pisz dobre wiadomości do commitu.

.. tip::

    Dobra wiadomość commita składa się z podsumowania (pierwsza linia),
    opcjonalnie po pustej lini dodaj więcej informacji. Podsumowanie
    powinno zaczynać się od nazwy komponentu (w nawiasie kwadratowym) nad którym pracujesz
    (``[DependencyInjection]``, ``[FrameworkBundle]``, ...).
    Użyj czasownika (``fixed ...``, ``added ...``, ...) aby rozpocząć podsumowanie
    oraz nie dodawaj kropki na końcu.

Wysyłanie Poprawki
------------------

Przed wysłaniem swojej poprawki, zaktualizuj swój branch (jest to potrzebne jeśli
przygotowanie zmiany zajeło Ci trochę czasu):

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout BRANCH_NAME
    $ git rebase master

Gdy wykonujesz komendę ``rebase``, mogą wystąpić konflikty które będziesz musiał rozwiązać.
``git status`` pokaże Ci **nie zmergowane** pliki. Po rozwiązaniu wszystkich konfliktów
możesz kontynuować:

.. code-block:: bash

    $ git add ... # dodaj pliki
    $ git rebase --continue

Sprawdź czy wszystkie testy przechodzą oraz wyślij swój branch na serwer zdalny:

.. code-block:: bash

    $ git push origin BRANCH_NAME

Teraz możesz podyskutować na temat swojej poprawki na `dev mailing-list`_ lub też zrobić
pull (musi być on wykonany na repozytorium ``symfony/symfony``). Aby ułatwić pracę
zespołowi, zawsze uwzględniaj modyfikowane komponenty w wiadomości pulla, jak poniżej:

.. code-block:: text

    [Yaml] foo bar
    [Form] [Validator] [FrameworkBundle] foo bar

Jeśli zamierzasz wysłać e-mail na listę mailingową, nie zapomnij dołączyć
URL do brancha (``https://github.com/NAZWA_UZYTKOWNIKA/symfony.git NAZWA_BRANCHA``)
lub też URL do pulla.

Na podstawie informacji zwrotnych z listy mailingowej lub w pullu na GitHub,
możliwe że będziesz musiał przerobić swoją poprawkę. Przed ponownym wysłaniem poprawki,
musisz wykonać rebase z branchem master, nie merguj; oraz wyślij poprawkę do źródła:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin BRANCH_NAME

.. note::

    Wszystkie poprawki które wysyłasz muszą być wydane zgodnie z licencją MIT,
    chyba że jest to wyraźnie określone w kodzie.

Wszystkie poprawki błędów zmergowane do utrzymywanych branchy są także mergowane
do aktualnych branchy. Dla przykładu, jeśli wyślesz poprawkę do brancha `2.0`,
poprawka zostanie dołączona przez zespół do brancha `master`.

.. _ProGit:              http://progit.org/
.. _GitHub:              https://github.com/signup/free
.. _repozytorium Symfony2: https://github.com/symfony/symfony
.. _dev mailing-list:    http://groups.google.com/group/symfony-devs
