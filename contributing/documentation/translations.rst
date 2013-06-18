Tłumaczenia
===========

Dokumentacja Symfony2 jest oryginalnie tworzona w języku angielskim, wielu ludzi
w społeczności jest zaangażowanych w tłumaczenia.

Udział
------

Przede wszystkim, zapoznaj się z :doc:`markup language <format>` używanym w
dokumentacji.

Potem, zasubskrybuj `Symfony docs mailing-list`_, ponieważ szczegóły współpracy
są omawiane właśnie tam.

Wreszcie, znajdź repozytorium *master* języka, na który chcesz tłumaczyć
dokumentację. Oto lista oficjalnych repozytoriów *master* tłumaczeń:

* *angielski*:  http://github.com/symfony/symfony-docs
* *francuski*:   https://github.com/gscorpio/symfony-docs-fr
* *włoski*:  https://github.com/garak/symfony-docs-it
* *japoński*: https://github.com/symfony-japan/symfony-docs-ja
* *polski*:   http://github.com/ampluso/symfony-docs-pl
* *portugalski (brazylijski)*:  https://github.com/andreia/symfony-docs-pt-BR
* *rumuński*: http://github.com/sebio/symfony-docs-ro
* *rosyjski*:  http://github.com/avalanche123/symfony-docs-ru
* *hiszpański*:  https://github.com/gitnacho/symfony-docs-es
* *turecki*:  https://github.com/symfony-tr/symfony-docs-tr

.. note::

    Jeśli chcesz wspomóc tłumaczenie dokumentacji na nowy język, przeczytaj
    :ref:`odpowiednią sekcję <translations-adding-a-new-language>`.

Dołączanie do ekipy tłumaczącej
-------------------------------

Jeśli chcesz wspomóc tłumaczenie dokumentacji dla swojego języka, dołącz do nas,
to bardzo prost proces:

* Przedstaw się na `Symfony docs mailing-list`_;
* *(opcjonalnie)* Spytaj nad którymi dokumentami możesz pracować;
* Zrób forka repozytorium *master* dla Twojego języka (kliknij w "Fork") na
  swojej stronie GitHuba);
* Przetłumacz kilka dokumentów;
* Poproś o pull request (kliknij w "Pull Request" na swojej stronie GitHuba);
* Lider zaakceptuje Twoje zmiany i zmerguje je do głównego repozytorium;
* Dokumentacja strony jest aktualizowana z głównego repozyrotium co noc.

.. _translations-adding-a-new-language:

Dodawanie nowego języka
-----------------------

Ta sekcja dostarcza kilka wskazówek dla rozpoczynania tłumaczenia dokumentacji
Symfony2 dla nowego języka.

Rozpoczynanie tłumaczenia to ogrom pracy, dlatego warto porozmawiać o swoich
planach na liście dyskusyjnej `Symfony docs mailing-list`_ i próbować znaleźć
ludzi zmotywowanych do pomocy.

Kiedy ekipa jest gotowa, należy wynaczyć lidera, który będzie odpowiedzialny
za repozytorium *master*.

Należy utworzyć repozytorium i skopiować wersję anglojęzyczną.

Ekipa może rozpocząć tłumaczenie dokumentacji.

Kiedy ekipa tłumacząca jest pewna, że repozytorium jest spójne i stabilne
(wszystko jest przetłumaczone, a nieprzetłumaczone dokumenty zostały usunięte
z toctrees -- files named ``index.rst`` and ``map.rst.inc``), lider może
poprosić o wpisanie repozytorium *master* na listę oficjalnych tłumaczeń,
wysyłając maila do Fabiena (fabien at symfony.com).

Utrzymywanie
------------

Tłumaczenie nie kończy się, kiedy wszystko zostanie przetłumaczone. Dokumentacja
się zmienia (nowe dokumenty są dodawane, błędy poprawiane, akapity
reorganizowane, ...). Ekipa tłumacząca powinna ściśle śledzić angielską wersję
i nanosić zmiany tak szybko jak to możliwe.

.. caution::

    Tłumaczenia nie utrzymywane są usuwane z listy oficjalnych tłumaczeń, jako
    że tłumaczenia przestarzałe wprowadzają użytkownika w błąd.

.. _Symfony docs mailing-list: http://groups.google.com/group/symfony-docs

