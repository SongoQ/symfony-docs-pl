.. index::
    single: Twig; rozszerzenie w Symfony2

Rozszerzenie Twig w Symfony2
============================

Twig jest domyślnym silnikiem szablonowania dla Symfony2. Sam w sobie, zawiera
wiele wbudowanych funkcji, filtrów, znaczników i testów (przejdź do
`http://twig.sensiolabs.org/documentation`_ i przewijaj w dół).

Symfony2 dodaje wiele własnym rozszerzeń do Twiga w celu integracji pewnych
komponentów z szablonami Twiga. Poniżej znajduje się informacja o wszystkich
niestandardowych funkcjach, filtrach, znacznikach i testach dodanych podczas
stosowania rdzenia frameworka Symfony2 Core Framework.

W pakietach mogą znajdować się też znaczniki, tutaj nie ujęte.

.. index::
    single: Twig; funkcje

Funkcje
-------

.. versionadded:: 2.2
    W symfony 2.2 zostały dodane nowe funkcje ``render`` i ``controller``.
    Poprzednio używany był znacznik ``{% render %}`` i miał on różne znaczenie.
    

+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| Składnia funkcji                                   | Zastosowanie                                                                                |
+====================================================+=============================================================================================+
| ``render(uri, options = {})``                      | Renderuje fragment dla danego kontrolera lub adresu URL.                                    |
| ``render(controller('B:C:a', {params}))``          | Aby uzyskać więcej informacji, zobacz:ref:`templating-embedding-controller`.                |
| ``render(path('route', {params}))``                |                                                                                             |
| ``render(url('route', {params}))``                 |                                                                                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``render_esi(controller('B:C:a', {params}))``      | Generuje znacznik ESI, gdy to możliwe, w przeciwnym razie powraca do zachowania             |
| ``render_esi(url('route', {params}))``             | ``render``. Aby uzyskać więcej informacji, zobacz:ref:`templating-embedding-controller`.    |
| ``render_esi(path('route', {params}))``            |                                                                                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``render_hinclude(controller(...))``               | Generuje znacznik Hinclude dla danego kontrolera lub adresu URL.                            |
| ``render_hinclude(url('route', {params}))``        | Aby uzyskać więcej informacji, zobacz :ref:`templating-embedding-controller`.               |
| ``render_hinclude(path('route', {params}))``       |                                                                                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``controller(attributes = {}, query = {})``        | Używa się wraz ze znacznikiem ``render`` do odwołania się do kontrolera, który wykorzystuje |
|                                                    | się do renderowania.                                                                        |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``asset(path, packageName = null)``                | Pobiera publiczną ścieżkę aktywa, więcej informacji w                                       |
|                                                    | ":ref:`book-templating-assets`".                                                            |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``asset_version(packageName = null)``              | Pobiera bieżącą wersję pakietu, więcej informacji w                                         |
|                                                    | ":ref:`book-templating-assets`".                                                            |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form(view, variables = {})``                     | Renderuje  kod HTML kompletnego formularza, więcej informacji w                             |
|                                                    | :ref:`reference-forms-twig-form`.                                                           |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_start(view, variables = {})``               | Renderuje kod HTML początkowego znacznika formularza, więcej informacji w                   |
|                                                    | :ref:`reference-forms-twig-start`.                                                          |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_end(view, variables = {})``                 | Renderuje kod HTML końcowego znacznika formularza wraz ze wszystkimi polami,                |
|                                                    | które nie zostały jeszcze zrenderowane, więcej informacji w                                 |
|                                                    | :ref:`reference-forms-twig-end`.                                                            |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_enctype(view)``                             | Renderuje obowiązkowy atrybut ``enctype="multipart/form-data"``                             |
|                                                    | jeśli formularz zawiera co najmnij jedno pole do pobierania pliku, więcej informacji w      |
|                                                    | :ref:`reference-forms-twig-enctype`.                                                        |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_widget(view, variables = {})``              | Renderuje kompletny formularz lub konkretny kod widgetu HTML pola,                          |
|                                                    | więcej informacji w :ref:`reference-forms-twig-widget`.                                     |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_errors(view)``                              | Renderuje wszystkie komunikaty o błędach dla danego pola lub błędach "globalnych",          |
|                                                    | więcej informacji w :ref:`reference-forms-twig-errors`.                                     |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_label(view, label = null, variables = {})`` | Renderuje etykietę dla określonego pola, więcej inforamcji w                                |
|                                                    | :ref:`reference-forms-twig-label`.                                                          |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_row(view, variables = {})``                 | Renderuje wiersz (etykietę pola, komunikaty błędów i widget) określonego pola,              |
|                                                    | więcej informacji w :ref:`reference-forms-twig-row`.                                        |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``form_rest(view, variables = {})``                | Renderuje wszystkie pola, które nie zostały jeszcze wyrenderowane, więcej informacji w      |
|                                                    | :ref:`reference-forms-twig-rest`.                                                           |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``csrf_token(intention)``                          | Renderuje token CSRF. Użyj tej funkcji, jeśli chcesz uzyskać ochoronę CSRF bez              |
|                                                    | tworzenia formularza                                                                        |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``is_granted(role, object = null, field = null)``  | Zwraca ``true`` jeśli bieżący użytkownik ma wymaganą rolę, więcej informacji w              |
|                                                    | ":ref:`book-security-template`"                                                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``logout_path(key)``                               | Generuje względną ścieżkę URL wylogowania dla określonej zapory                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``logout_url(key)``                                | Równoważnik ``logout_path(...)`` ale generuje bezwzględny adres URL                         |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``path(name, parameters = {})``                    | Pobiera względną ścieżkę URL dla danej trasy, więcej informacji w                           |
|                                                    | ":ref:`book-templating-pages`".                                                             |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+
| ``url(name, parameters = {})``                     | Równoważnik ``path(...)`` ale generuje bezwzględny adres URL                                |
+----------------------------------------------------+---------------------------------------------------------------------------------------------+

.. index::
    single: Twig; filtry

Filtry
------

+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| Składnia fitra                                                                  | Zastosowanie                                                    |
+=================================================================================+=================================================================+
| ``text humanize``                                                               | Czyni techniczne nazwy czytelnymi dla człowieka (zamienia znaki |
|                                                                                 | kreski dolnej spacjami i kapitalizuje litery w łańcuchu)        |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``text trans(arguments = {}, domain = 'messages', locale = null)``              | Tłumaczy tekst na bieżacy język,                                |
|                                                                                 | więcej informacji w :ref:`book-translation-filters`.            |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``text transchoice(count, arguments = {}, domain = 'messages', locale = null)`` | Tłumaczy tekst z jego pluralizacją (zastosowaniem formy liczby  |
|                                                                                 | mnogiej), więcej inforamcji w :ref:`book-translation-filters`.  |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``variable yaml_encode(inline = 0)``                                            | Transformuje zmienną tekstową do składni YAML.                  |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``variable yaml_dump``                                                          | Renderuje kod YAML z jego wypisaniem.                           |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``classname abbr_class``                                                        | Renderuje element ``abbr`` z krótką nazwą klasy PHP klasy PHP   |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``methodname abbr_method``                                                      | Renderuje nazwę metody PHP wewnątrz elementu ``abbr``           |
|                                                                                 | (np. ``Symfony\Component\HttpFoundation\Response::getContent``  |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``arguments format_args``                                                       | Renderuje łańcuch tekstowy z argumentami funkcji i to wypisuje. |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``arguments format_args_as_text``                                               | Równoważne ``[...] format_args``, ale wycina znaczniki.         |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``path file_excerpt(line)``                                                     | Renderuje fragment pliku z kodem otoczony podaną linią.         |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``path format_file(line, text = null)``                                         | Renderuje ścieżkę pliku w linku.                                |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``exceptionMessage format_file_from_text``                                      | Równoważne ``format_file`` ale parsuje typowy dla błędu PHP     |
|                                                                                 | łańcuch ze ścieżką pliku (tzn. 'in foo.php on line 45')         |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+
| ``path file_link(line)``                                                        | Renderuje ścieżkę do właściwego pliku  (z numerem linii)        |
+---------------------------------------------------------------------------------+-----------------------------------------------------------------+

.. index::
    single: Twig; znaczniki

Znaczniki
---------

+---------------------------------------------------+-------------------------------------------------------------------+
| Tag Syntax                                        | Zastosowanie                                                      |
+===================================================+===================================================================+
| ``{% form_theme form 'file' %}``                  | Wyszukuje w określonym pliku nadpisujące bloki formularzowe,      |
|                                                   | więcej informacji w :doc:`/cookbook/form/form_customization`.     |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans with {variables} %}...{% endtrans %}`` | Tłumaczy i renderuje tekst, więcej informacji w                   |
|                                                   | :ref:`book-translation-tags`                                      |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% transchoice count with {variables} %}``      | Tłumaczy i renderuje tekst z jego pluralizacją (dostosowaniem do  |
| ...                                               | liczby mnogiej), więcej informacji w :ref:`book-translation-tags` |
| ``{% endtranschoice %}``                          |                                                                   |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans_default_domain language %}``           | Ustawia domyślną domenę dla katalogów komunikatów w bieżącym      |
|                                                   | szablonie                                                         |
+---------------------------------------------------+-------------------------------------------------------------------+

.. index::
    single: Twig; testy

Testy
-----

+-------------------------------------------+-----------------------------------------------------------------------------+
| Test Syntax                               | Zastosowanie                                                                |
+===========================================+=============================================================================+
| ``selectedchoice(choice, selectedValue)`` | Zwraca ``true``, jeśli zaznaczony jest wybór dla danej wartości formularza. |
+-------------------------------------------+-----------------------------------------------------------------------------+

.. index::
    single: Twig; zmienne globalne

Zmienne globalne
----------------

+-----------------------------------------------------+---------------------------------------------------------------------------+
| Zmienna                                             | Zastosowanie                                                              |
+=====================================================+===========================================================================+
| ``app`` *Attributes*: ``app.user``, ``app.request`` | Zmienna ``app`` jest dostępna wszędzie i umożliwia szybki dostęp do wielu |
| ``app.session``, ``app.environment``, ``app.debug`` | najczęściej potrzebnych obiektów. Zmienna ``app`` jest instancją klasy    |
| ``app.security``                                    | :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`    |
+-----------------------------------------------------+---------------------------------------------------------------------------+

.. index::
    single: Twig; rozszerzenia dla Symfony2 SE

Rozszerzenia Symfony Standard Edition
-------------------------------------

Symfony Standard Edition dodaje kilka pakietów do Symfony2 Core Framework.
Pakiety te mogą mieć inne rozszerzenia Twig:

* **Rozszerzenie Twig** dołącza wszystkie rozszerzenia, które nie należą do rdzenia
  Twig, ale mogą być interesujace. Czytaj więcej na ten temat w
  `oficjalnej dokumentacji rozszerzeń Twig`_
* **Assetic** dodaje znaczniki ``{% stylesheets %}``, ``{% javascripts %}`` i 
  ``{% image %}``. Mozna przeczytać więcej na ten temat w 
  :doc:`dokumentacji Assetic </cookbook/assetic/asset_management>`;

.. _``oficjalnej dokumentacji rozszerzeń Twig`: http://twig.sensiolabs.org/doc/extensions/index.html
.. _`http://twig.sensiolabs.org/documentation`: http://twig.sensiolabs.org/documentation
