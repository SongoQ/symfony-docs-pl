.. index::
   single: pakiet; instalacja

Jak zainstalować pakiety firm trzecich ?
========================================

Większość pakietów zapewnia swoje instrukcje obsługi. Jednakże, podstawowe
etapy instalacji pakietów są niemalże identyczne.

Dodawanie zależności w Composer
-------------------------------

Począwszy od Symfony 2.1, zależności są zarządzane przez Composer. To dobry
pomysł, aby nauczyć się podstaw Composera studiując `jego dokumentację`_.

Przed użyciem Composera do instalacji pakietu, należy sprawdzić czy dany
pakiet istnieje w archiwum `Packagist`_. Na przykład, jeśli wyszukiwano
popularnego `FOSUserBundle`_, archiwum powinno odnaleźć `friendsofsymfony/user-bundle`_.

.. note::

    Packagist jest głównym archiwum dla Composera. Jeśli szukasz pakietu,
    najlepsze co możesz zrobić, to sprawdzić serwis `KnpBundles`_, gdyż
    jest to nieoficjalne archiwum pakietów Symfony. Jeśli pakiet zawiera
    plik ``README``, zostanie wyświetlony, a jeśli posiada wpis w Packagist,
    zostanie dodatkowo ukazany odnośnik do owej paczki. To naprawdę przydatna
    witryna, od której warto rozpocząć poszukiwania konkretnych pakietów.

Teraz, gdy ma się już nazwę pakietu, należy określić, którą wersję użyć.
Zazwyczaj różne wersje pakietu odpowiadają konkretnej wersji Symfony. Informacje
te powinny być zawarte w pliku ``README``. Jeśli tak nie jest, można użyć
dowolnej wersji. Jeśli przez przypadek wybrano niezgodną, Composer poinformuje
o błędnych zależnościach, które planowano zainstalować. Jeśli to się wydarzy,
można spróbować innej wersji.

W przypadku FOSUserBundle, plik ``README`` zawiera przestrogę, że wersja
1.2.0 musi być używana z Symfony 2.0, a 1.3+ z Symfony 2.1+. Packagist wyświetla
przykładowe wymagania ``require`` dla wszystkich wersji pakietu. Aktualna
wersja rozwojowa FOSUserBundle to ``"friendsofsymfony/user-bundle": "2.0.*@dev"``.

Teraz można dodać pakiet do pliku ``composer.json`` i zaktualizować zależności.
Można zrobić to ręcznie:

1. **Dodaj poniższe linie do pliku composer.json:**

   .. code-block:: json
      :linenos:

       {
           ...,
           "require": {
               ...,
               "friendsofsymfony/user-bundle": "2.0.*@dev"
           }
       }

2. **Zaktualizuj zależności**

   .. code-block:: bash
      :linenos:

       $ php composer.phar update friendsofsymfony/user-bundle

   albo zaktualizuj wszystkie zależności naraz

   .. code-block:: bash
      :linenos:

       $ php composer.phar update

Można to również zrobić jednym poleceniem:

.. code-block:: bash
   :linenos:

    $ php composer.phar require friendsofsymfony/user-bundle:2.0.*@dev

Aktywowanie pakietu
-------------------

W tym momencie pakiet jest zainstalowany w projekcie Symfony (w ``vendor/friendsofsymfony/``),
a autoloader rozpoznaje jego klasy. Jedyne co trzeba zrobić, to zarejestrować
pakiet w ``AppKernel``::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function registerBundles()
        {
            $bundles = array(
                // ...,
                new FOS\UserBundle\FOSUserBundle(),
            );

            // ...
        }
    }

Konfigurowanie pakietu
----------------------

Pakiet zazwyczaj wymaga dodania specjalnej konfiguracji do pliku ``app/config/config.yml``.
Dokumentacja pakietu najprawdopodobniej opisze wszelkie szczegóły, niemniej
można również odwołać się do jego konfiguracji używając polecenia ``config:dump-reference``.

Na przykład, aby zobaczyć odwołania do konfiguracji ``assetic``, można użyć:

.. code-block:: bash
   :linenos:

    $ app/console config:dump-reference AsseticBundle

albo też:

.. code-block:: bash
   :linenos:

    $ app/console config:dump-reference assetic

Na wyjściu powinno się otrzymać coś podobnego do:

.. code-block:: text
   :linenos:

    assetic:
        debug:                %kernel.debug%
        use_controller:
            enabled:              %kernel.debug%
            profiler:             false
        read_from:            %kernel.root_dir%/../web
        write_to:             %assetic.read_from%
        java:                 /usr/bin/java
        node:                 /usr/local/bin/node
        node_paths:           []
        # ...

Inne ustawienia
---------------

W tym momencie powinno się przestudiować plik ``README`` używanego pakietu i
zobaczyć co zrobić dalej.

.. _jego dokumentację: http://getcomposer.org/doc/00-intro.md
.. _Packagist:           https://packagist.org
.. _FOSUserBundle:       https://github.com/FriendsOfSymfony/FOSUserBundle
.. _`friendsofsymfony/user-bundle`: https://packagist.org/packages/friendsofsymfony/user-bundle
.. _KnpBundles:          http://knpbundles.com/
