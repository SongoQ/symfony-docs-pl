.. symfony-cms-docs-pl Documentation master file.

.. index::
   single: Symfony CMF SE

************************
Dokumentacja Symfony CMF
************************

Jest to polska dokumentacja dystrybucji frameworka Symfony o nazwie
`Symfony Content Management Framework`_. Dystrybucję tą będziemy w skrócie
nazywać *Symfony CMF*, nie wdając się teraz w dywagacje, co to jest *Content Management
Framework*. Nadmienię tu tylko, że do tego typu platform programistycznych
(opartych na PHP) zalicza sie takie systemy jak `Drupal`_, `ez Publish`_  czy
`MODx`_. Temat ten rozwinięty jest w sekcji :ref:`why-another-cms`.

Dokumentacja ta w zasadzie jest tłumaczeniem oficjalnej dokumentacji
`Symfony Content Management Framework`_, publikowanej na stronie
http://symfony.com/doc/master/cmf/, ale w założeniach ma zawierać też oryginalne
artykuły polskojęzyczne.    
 
Projekt *Symfony2 Content Management Framework* jest realizowany przez
społeczność Symfony i ma kilka firm sponsorujących oraz wsparcie czołowych
liderów otwartego oprogramowania upowszechniających koncepcję `rozdzielonego CMS`_
(*ang. decoupled CMS*). Możesz dowiedzieć się więcej o projekcie czytając
stronę `about`_.

Dokumentacja ta (zarówno angielski oryginał jak i polskie tłumaczennie) są jeszcze 
w fazie tworzenia i dalekie są od skończenia. Jeśli chodzi o oryginalną,
angielskojezyczną dokumentację, to proszę przeczytać dokument
`Documentation planning`_ w celu zapoznania się z planowanymi pracami przy tej wersji.
Plan pracy z polską dokumentacją opisany jest w sekcji :ref:`docs-pl-roadmap`.
Chcesz pomóc? Dzięki, wszelka pomoc jest mile widziana. Proszę o zapoznanie się
z sekcją :ref:`contributing-to-docs`.  
 

Misja
-----

    Projekt Symfony CMF ułatwia programistom dodawać funkcjonalność CMS do aplikacji
    zbudowanej z frameworka PHP Symfony2. Kluczowe pryncypia tworzenia aplikacji
    związane ze zbiorem pakietów, to skalowalność, użyteczność, dokumentacja
    i testowanie.

.. _why-another-cms:

Dlaczego jeszcze jeden CMS?
---------------------------

Właściwie uważamy, że ten projekt jest **frameworkiem systemu zarządzania treścią**
(*ang. Content Management Framework - CMF*) a nie **systemem zarządzania treścią**
(*ang. Content Management System - CMS*). Powodem tego jest fakt, że tylko
**dostarczamy narzędzia do budowy własnego CMS**. Istnieje wiele rozwiązań CMS już
gotowych, ale na ogół są to monolityczne pakiety dostosowane do potrzeb końcowych
użytkowników. Wiele z nich posiada bagaż starszych rozwiązań, co czyni je mniej
niż **idealnymi do tworzenia wysoko zindywidualizowanych aplikacji**, w przeciwieństwie
do `Symfony2`_.

Do kogo kierowany jest ten system?
----------------------------------

Istnieją dwie główne grupy odbiorców:

#. Programiści, którzy mają zbudowane już aplikacje z Symfony2 i potrzebują
   szybkiego sposobu na dodanie obsługi zarządzania treścią. Czy to będą zaawansowane
   funkcjonalności CMS, takie jak treść znaczeniowa, edycja wierszowa dokumentu,
   dostawa wielokanałowa itd., lub tylko kilka stron zawartości dla takich elementów
   jak strona „O nas” lub „Kontakt”.

#. Programiści, którzy potrzebują zbudować wysoko zindywidualizowane autorskie
   rozwiązanie dostarczania treści, które nie jest gotową aplikacją CMS przeznaczoną
   do rozpowszechniania jako gotowy system, ewentualnie możliwy do dostosowania
   przez użytkownika.


:doc:`/cmf/getting_started/index`
---------------------------------

Właśnie rozpocząłeś naukę o CMF? Chcesz dowiedzieć się czy CMF nadaje się do Twojego
pierwszego projektu? Rozpocznij tutaj.

.. include:: /cmf/getting_started/maps.rst.inc 

Poradniki
---------

Chcesz dowiedzieć się więcej o CMF i o tym jak każda część systemu może zostać
skonfigurowana? Te poradniki są dla każdego.

.. toctree::
   :maxdepth: 1

   tutorials/choosing_a_storage_layer
   tutorials/installing_cmf_core
   tutorials/installing_configuring_doctrine_phpcr_odm
   tutorials/installing_configuring_inline_editing
   tutorials/creating_cms_using_cmf_and_sonata
   tutorials/using_blockbundle_and_contentbundle
   tutorials/handling_multilang_documents

Pakiety
-------

Szukasz szczegółowszych informacji o pakietach CMF? Potrzebujesz poznać listę
wszystkich opcji konfiguracyjnych pakietu? Chcesz wiedzieć, czy można stosować
pakiet niezależnie i jak to zrobić? W takim przypadku to miejsce jest dla Ciebie.

.. include:: /cmf/bundles/map.rst.inc
   
Receptariusz
------------

Specjalne rozwiązania dla specjalnych potrzeb, które wykraczają poza standardowe
zastosowania.

.. toctree::
   :maxdepth: 1

   cookbook/phpcr_odm_custom_documentclass_mapper
   cookbook/using_a_custom_route_repository
   cookbook/installing_cmf_sandbox

Komponenty
----------

Szukasz jakiejś informacji o komponentach niskiego poziomu Symfony CMF?

.. toctree::
   :maxdepth: 1

   components/routing

Współpraca
----------

.. toctree::
   :maxdepth: 1

   contributing/code
   contributing/license
   contributing/docs

.. _`ez Publish`: http://pl.wikipedia.org/wiki/EZ_publish
.. _`Drupal`: http://pl.wikipedia.org/wiki/Drupal
.. _`MODx`: http://modx.com/ 
.. _`rozdzielonego CMS`: http://decoupledcms.org
.. _`Symfony2`: http://symfony.com
.. _`about`: http://cmf.symfony.com/about
.. _`Documentation planning`: https://github.com/symfony-cmf/symfony-cmf/wiki/Documentation-Planning
.. _`Symfony Content Management Framework`: http://cmf.symfony.com
.. _`github`: https://github.com/symfony-cmf/symfony-cmf-docs

Indeksy i tabele
================
* :ref:`genindex`
* :ref:`search`
