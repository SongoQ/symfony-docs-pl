.. highlight:: php
   :linenothreshold: 2

.. index::
   single: pakiety; SensioGeneratorBundle
   single: SensioGeneratorBundle
   
SensioGeneratorBundle
=====================

Pakiet **SensioGeneratorBundle** rozszerza domyślny interfejs wiersza poleceń Symfony2
dostarczając nowych interaktywnych i intuicyjnych poleceń do generowania szkieletów
kodu, takich jak pakiety, klasy formularzy lub kontrolery CRUD oparte na schemacie
Doctrine 2.

.. index::
   single: SensioGeneratorBundle; instalacja

Instalacja
----------

`Pobierz`_ pakiet i umieść go w przestrzeni nazw Sensio\\Bundle\\. Następnie zarejestruj
go w klasie Kernel, tak jak każdy inny pakiet::
   
   public function registerBundles()
   {
      $bundles = array(
         ...
         
         new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle(),
      );
      
      ...
   }

Lista dostępnych poleceń
------------------------

Pakiet *SensioGeneratorBundle* dostarczany jest z czterema nowymi poleceniami,
które można uruchamiać w trybie interaktywnym lub zwykłym. W trybie interaktywnym
program zadaje użytkownikowi kilka pytań w celu skonfigurowania parametrów polecenia
niezbędnych do wygenerowania ostatecznego kodu. Oto lista nowych poleceń:

.. toctree::
   :maxdepth: 1
   
   commands/generate_bundle
   commands/generate_controller
   commands/generate_doctrine_crud
   commands/generate_doctrine_entity
   commands/generate_doctrine_form

.. _`Pobierz`: http://github.com/sensio/SensioGeneratorBundle

Przesłanianie szablonów szkieletowych
-------------------------------------

.. versionadded:: 2.3
   W wersji 2.3 dodano możliwość przesłaniania szablonów szkieletowych.

Wszystkie generatory używają szablonu szkieletowego do generowania plików.
Domyślnie polecenia używają szablonów dostarczonych w pakietach, w ich katalogu
``Resources/skeleton``.

Można zdefiniować własne szablony szkieletowe tworząc ten sam katalog i strukturę
pliku w ``APP_PATH/Resources/SensioGeneratorBundle/skeleton`` lub
``BUNDLE_PATH/Resources/SensioGeneratorBundle/skeleton`` jeśli chce się rozszerzyć
pakiet generatora (aby na przykład umożliwić współdzielenie szablonów w różnych
projektach).

Na przykład, jeśli chce się przesłonić szablon ``edit`` dla generatora CRUD, trzeba
utworzyć plik ``crud/views/edit.html.twig.twig`` w katalogu
``APP_PATH/Resources/SensioGeneratorBundle/skeleton``.

Kiedy przesłaniasz szablon, rzuć okiem na domyślne szablony, aby dowiedzieć się
więcej o dostępnych szablonach, ich ścieżkach i zmiennych do których mają dostęp.

Zamiast kopiować/wklejać oryginalny szablon w celu stworzenia własnego, można go
rozszerzyć i tylko przesłonić odpowiednie części:

.. code-block:: jinja
   :linenos:
   
   {# in app/Resources/SensioGeneratorBundle/skeleton/crud/actions/create.php.twig #}
   
   {# notice the "skeleton" prefix here -- more about it below #}
   {% extends "skeleton/crud/actions/create.php.twig" %}
   
   {% block phpdoc_header %}
       {{ parent() }}
       *
       * To ma być wstawione po tytule phpdoc, ale przed adnotacjami.
       *
   {% endblock phpdoc_header %}

Skomplikowane szablony w domyślny, szkielecie są podzielone na bloki Twiga, aby
umożliwić łatwe dziedziczenie i uniknąć kopiowania/wklejania dużych porcji kodu.

W niektórych przypadkach szablony w szkielecie dołączają inne szablony, jak
na przykład szablon ``crud/views/edit.html.twig.twig``:

.. code-block:: jinja

   {% include 'crud/views/others/record_actions.html.twig.twig' %}

Jeśli zdefiniowano własny szablon dla tego szablonu, to zostanie on użyty zamiast
szablonu domyślnego. Ale można jawnie dołączyć oryginalny szablon szkieletu poprzedzając
jego ścieżkę  przedrostkiem ``skeleton/``, tak jak poniżej:

.. code-block:: jinja
   
   {% include 'skeleton/crud/views/others/record_actions.html.twig.twig' %}

Można dowiedzieć się więcej o takich super "trikach" w oficjalnej
`dokumentacji Twig <http://twig.sensiolabs.org/doc/recipes.html#overriding-a-template-that-also-extends-itself>`_.
