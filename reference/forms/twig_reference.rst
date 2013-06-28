.. index::
   single: formularze; funkcje formularzowe Twig

Opis funkcji i zmiennych szablonów formularzowych Twig
======================================================

Podczas pracy z formularzami w szablonie, ma się do dyspozycji dwa potężne narzędzia:

* :ref:`funkcje<reference-form-twig-functions>` do renderowania każdej części formularza;
* :ref:`zmienne<twig-reference-form-variables>` do pobierania każdej informacji o jakimkowiek polu.

Funkcje wykorzystuje się powszechnie do renderowania pól. Natomiast zmienne są tak
powszechnie używane, ale są bardzo użyteczne, ponieważ umożliwiają dostęp do etykiety
pól, atrybutu id, komunikatów błędów i wszystkiego, co jest związane z polem.

.. _reference-form-twig-functions:

Funkcje renderowania formularza
-------------------------------

Ten informator obejmuje wszystkie możliwe funkcje Twig dostępne dla renderowania
formularzy. Dostępnych jest kilka różnych funkcji i każda z nich jest odpowiedzialna
za renderowanie innej części formularza (np. etykiety, komunikaty błedów, widgety itd.).

.. _reference-forms-twig-form:

form(view, variables)
---------------------

Renderuje kod HTML kompletnego formularza.

.. code-block:: jinja
   :linenos:

    {# render the form and change the submission method #}
    {{ form(form, {'method': 'GET'}) }}

Najczęściej używa się tej funkcji pomocniczej do prototypowania lub jeśli używa
się indywidualnych motywów formularza. Jeśli potrzeba większej elastyczności
w renderowaniu formularza, to zamiast tego należy użyć innych funkcji pomocniczych
do renderowania indywidualnych części formularza:

.. code-block:: jinja
   :linenos:

    {{ form_start(form) }}
        {{ form_errors(form) }}

        {{ form_row(form.name) }}
        {{ form_row(form.dueDate) }}

        <input type="submit" value="Submit me"/>
    {{ form_end(form) }}

.. _reference-forms-twig-start:

form_start(view, variables)
---------------------------

Renderuje początkowy znacznik formularza. Ta funkcja pomocnicza drukuje skonfigurowaną
metodę i docelową akcję formularza. Dołącza również prawidłową właściwość ``enctype``,
jeśli  formularz zawiera pola ładujące dane.

.. code-block:: jinja
   :linenos:

    {# render the start tag and change the submission method #}
    {{ form_start(form, {'method': 'GET'}) }}

.. _reference-forms-twig-end:

form_end(view, variables)
-------------------------

Renderuje znacznik końcowy formularza.

.. code-block:: jinja

    {{ form_end(form) }}

Ta funkcja pomocnicza wyprowadza również ``form_rest()``, chyba że ``render_rest``
ustawiono na ``false``:

.. code-block:: jinja
   :linenos:

    {# don't render unrendered fields #}
    {{ form_end(form, {'render_rest': false}) }}

.. _reference-forms-twig-label:

form_label(view, label, variables)
----------------------------------

Renderuje etykietę dla danego pola. Jako drugi argument można opcjonalnie przekazać
konkretną etykietę, którą chce się wyświetlić.

.. code-block:: jinja
   :linenos:

    {{ form_label(form.name) }}

    {# The two following syntaxes are equivalent #}
    {{ form_label(form.name, 'Your Name', {'label_attr': {'class': 'foo'}}) }}
    {{ form_label(form.name, null, {'label': 'Your name', 'label_attr': {'class': 'foo'}}) }}

Zapoznaj sie z ":ref:`twig-reference-form-variables`" aby dowiedzieć się więcej
o argumencie ``variables``.

.. _reference-forms-twig-errors:

form_errors(view)
-----------------
    
Renderuje każdy komunikat błędu dla danego pola.

.. code-block:: jinja
   :linenos:

    {{ form_errors(form.name) }}

    {# wyrenderowanie komunikatów błedów globalnych #}
    {{ form_errors(form) }}


.. _reference-forms-twig-widget:

form_widget(view, variables)
----------------------------

Renderuje widget HTML dla danego pola. Jeśli zastosuje się to do całego formularza,
lub kolekcji pól, to zrenderowane zostanie każdy wiersz w tym formularzu lub kolekcji
pól.

.. code-block:: jinja
   :linenos:

    {# wyrenderowanie widgetu, ale z dodaniem do nigo klasy "foo" #}
    {{ form_widget(form.name, {'attr': {'class': 'foo'}}) }}

Drugim argumentem funkcji ``form_widget`` jest tablica zmiennych. Najbardziej
popularną zmienną jest ``attr``, która jest tablicą atrybutów HTML mających być
zastosowanymi w widgecie. W niektórych przypadkach, niektóre typy posiadają także 
inne opcje związane z szablonem, które mogą zostać przekazane.
Są one omówione w informacjach o typach. Tablica ``attributes`` nie jest stosowana
rekursywnie dla pól podrzednych, jeśli renderuje się wiele pól w jednym czasie
(np. ``form_widget(form)``).

Zobacz ":ref:`twig-reference-form-variables`", aby dowiedzieć się więcej o argumencie
``variables``.

.. _reference-forms-twig-row:

form_row(view, variables)
-------------------------

Renderuje "wiersz" danego pola, który jest kombinacją etykiety pola, komunikatu
błędu i widgetu.

.. code-block:: jinja
   :linenos:

   {# wyrenderowanie wiersza pola, ale wyświetlenie etykiety z tekstem "foo" #}
   {{ form_row(form.name, {'label': 'foo'}) }}

Drugim argumentem ``form_row`` jest tablica zmiennych. Szablony dostarczone
w Symfony pozwalają tylko zastąpić etykietę, tak jak to jest pokazano to
w przykładzie powyżej.

Zobacz ":ref:`twig-reference-form-variables`" w celu uzupełnienia wiedzy o argumencie
``variables``.

.. _reference-forms-twig-rest:

form_rest(view, variables)
--------------------------

Funkcja ta renderuje wszystkie pola które jeszcze nie zostały wyrenderowane
dla danego formularza. Dobrym pomysłem jest wykorzystanie tej funkcji w środku
formularza, ponieważ funkcja ta renderuje ukryte pola oraz pokazuje w oczywisty
sposób które pola zostały przez pominięte (poprzez ich wyrenderowanie).

.. code-block:: jinja

    {{ form_rest(form) }}

.. _reference-forms-twig-enctype:

form_enctype(view)
------------------

.. note::

    Ta funkcja pomocnicza została zdeprecjonowana w Symfony 2.3 i zostanie usunięta
    w Symfony 3.0. Zmiast niej używaj ``form_start()``.


Jeśli formularz posiada przynajmniej jedno pole z możliwością przesłania pliku,
funkcja ta wyrenderuje wymagany atrybut ``enctype="multipart/form-data"``.

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>
   
.. _`twig-reference-form-variables`:

Więcej o zmiennych formularza
-----------------------------

.. tip::

    Aby poznać pełną listę zmiennych, zobacz: :ref:`reference-form-twig-variables`.

W prawie każdej powyzszej funkcji Twiga ostatni argument jest tablicą "zmiennych"
używanych podczas renderowania poszczególnych części formularza. Na przykład,
poniższy kod renderuje "widget" dla pola i modyfikuje jego atrybuty w celu dołączenie
konkretnej klasy:

.. code-block:: jinja
   :linenos:

    {# wyrenderowanie widgetu i dodanie do niego klasy "foo" #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

Przeznaczenie tych zmiennych – co robią i skąd się biorą – mogą nie być od razu
jasne, ale mają one bardzo duże możliwości. Podczas renderowania każdej części
formularza, renderowany blok wykorzystuje wiele zmiennych. Domyślnie bloki te
znajdują się w `form_div_layout.html.twig`_.

Dla przykładu rozpatrzmy ``form_label``:

.. code-block:: jinja
   :linenos:

    {% block form_label %}
        {% if not compound %}
            {% set label_attr = label_attr|merge({'for': id}) %}
        {% endif %}
        {% if required %}
            {% set label_attr = label_attr|merge({'class': (label_attr.class|default('') ~ ' required')|trim}) %}
        {% endif %}
        {% if label is empty %}
            {% set label = name|humanize %}
        {% endif %}
        <label{% for attrname, attrvalue in label_attr %} {{ attrname }}="{{ attrvalue }}"{% endfor %}>{{ label|trans({}, translation_domain) }}</label>
    {% endblock form_label %}

Blok ten wykorzystuje kilka zmiennych: ``compound``, ``label_attr``, ``required``,
``label``, ``name`` i ``translation_domain``. Dostępne są one przez system renderowania
formularza. Lecz dużo ważniejsze są zmienne, które można nadpisać podczas wywołania
``form_label`` (ponieważ w tym przykładzie renderowana jest etykieta).

To jakie zmienne są dostępne do napisania zależy od tego, która część formularza
jest renderowana (np. etykieta versus widget) i jakie pole jest renderowane
(np. widget ``choice`` ma dodatkową opcję ``expanded``).
Przeglądając `form_div_layout.html.twig`_, można zobaczyć jakie opcje są dostępne.

.. tip::

    W tle, zmienne te są udostępniane dla obiektu ``FormView`` formularza, gdy
    komponent formularza wywołuje ``buildView`` i ``buildViewBottomUp`` na każdym
    "węźle" drzewa formularza. Aby zobaczyć co mają szczególnego zmienne "view",
    znajdź kod źródłowy dla pola formularza (i jego pól nadrzędnych) i przeglądnij
    dwie powyższe funkcje.

.. note::

    Jeżeli renderuje się naraz cały formularz (lub cały osadzony formularz), argument
    ``variables`` zostanie zastosowany tylko do samego formularza a nie do formularzy
    potomnych. Innymi słowy, w poniższym kodzie atrybut klasy "foo" *nie* zostanie
    przekazany do wszystkich pól potomnych w formularzu:

    .. code-block:: jinja

        {# to **nie** działa - zmienne nie są rekursywane #}
        {{ form_widget(form, { 'attr': {'class': 'foo'} }) }}

.. _reference-form-twig-variables:

Informacje o zmiennych formularza
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Poniższe zmienne są wspólne dla wszystkich typów pól. Niektóre typy pól mogą mieć
więcej zmiennych a tutaj zastosowano tylko niektóre z nich.

Załóżmy, że mamy w szablonie zmienną ``form`` i chcemy odwołać się do zmiennej w
polu ``name``, to udostępnieie zmiennych jest realizowane przez użycie publicznej
właściwości ``vars`` w obiekcie :class:`Symfony\\Component\\Form\\FormView`:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <label for="{{ form.name.vars.id }}"
            class="{{ form.name.vars.required ? 'required' : '' }}">
            {{ form.name.vars.label }}
        </label>

    .. code-block:: html+php
       :linenos:

        <label for="<?php echo $view['form']->get('name')->vars['id'] ?>"
            class="<?php echo $view['form']->get('name')->vars['required'] ? 'required' : '' ?>">
            <?php echo $view['form']->get('name')->vars['label'] ?>
        </label>

+----------------+----------------------------------------------------------------------------------------+
| Zmienna        | Zastosowanie                                                                           |
+================+========================================================================================+
| ``id``         | Atrybut  HTML ``id`` do wyrenderowania.                                                |
+----------------+----------------------------------------------------------------------------------------+
| ``name``       | Nazwa pola (np. ``title``) - ale nie nazwa atrybutu HTML ``name``, która jest ustalana |
|                | w zmiennej ``full_name``.                                                              |
+----------------+----------------------------------------------------------------------------------------+
| ``full_name``  | Atrybut HTML ``name`` do wyrenderowania.                                               |
+----------------+----------------------------------------------------------------------------------------+
| ``errors``     | Tablica jakichkolwiek komunikatów błędów związanych z *tym* określonym polem           |
|                | (np. ``form.title.errors``). Należy pamiętać, że nie można używać ``form.errors``      |
|                | do określania, czy formularz jest prawidłowy, ponieważ zwraca tylko komunikaty błędów  |
|                | "globalnych": niektóre indywidualne pola mogą mieć komunikaty błedów. Zamiast tego     |
|                | trzeba użyć opcji ``valid``.                                                           |
+----------------+----------------------------------------------------------------------------------------+
| ``valid``      | Zwraca ``true`` lub ``false`` w zależności od tego, czy cały formularz jest właściwy.  |
+----------------+----------------------------------------------------------------------------------------+
| ``value``      | Wartość, która będzie użyta podczas renderowania  (zwykle atrybut HTML ``).            |
+----------------+----------------------------------------------------------------------------------------+
| ``read_only``  | Jeśli ``true``, do pola dodawane jest ``readonly="readonly"``.                         |
+----------------+----------------------------------------------------------------------------------------+
| ``disabled``   | Jeśli ``true``, do pola dodawane jest ``disabled="disabled"``.                         |
+----------------+----------------------------------------------------------------------------------------+
| ``required``   | Jeśli ``true``, to do pola zostanie dodany atrybut ``required`` w celu aktywowania     |
|                | walidacji HTML5. Dodatkowo do etykiety dodawana jest klasa ``required``.               |
+----------------+----------------------------------------------------------------------------------------+
| ``max_length`` | Dodaje do elementu atrybut HTML ``maxlength``.                                         |
+----------------+----------------------------------------------------------------------------------------+
| ``pattern``    | Dodaje do elementu atrybut HTM ``pattern``.                                            |
+----------------+----------------------------------------------------------------------------------------+
| ``label``      | Łańcuch tekstowy etykiety do wyrenderowania.                                           |
+----------------+----------------------------------------------------------------------------------------+
| ``multipart``  | Jeśli ``true``, to ``form_enctype`` wyrenderuje ``enctype="multipart/form-data"``.     |
|                | Ma to zastosowanie tylko do głównego elementu formularza.                              |
+----------------+----------------------------------------------------------------------------------------+
| ``attr``       | Tablica par klucz-wartość do wyrenderowania jako atrybuty HTML pola.                   |
+----------------+----------------------------------------------------------------------------------------+
| ``label_attr`` | Tablica par klucz-wartość do wyrenderowania jako atrybuty HTML etykiety.               |
+----------------+----------------------------------------------------------------------------------------+
| ``compound``   | Wskazuje, czy pole w rzeczywistości posiada grupę pól potomnych.                       |
|                | Na przykład, pole ``choice`` , które jest w rzeczywistości grupą pól wyboru.           |
+----------------+----------------------------------------------------------------------------------------+

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
 