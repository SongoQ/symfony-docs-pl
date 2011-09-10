.. index::
   single: Forms; Twig form function reference

Funkcje Formularza w Twig
=========================

Ten podręcznik obejmuje wszystkie możliwe funkcje Twig dostępne dla
renderowania formularzy. Dostępnych jest kilka funkcji, każda z nich jest
odpowiedzialna za renderowanie innej części formularza (np. etykiety, błędy,
widgety, etc.).

form_label(form.name, label, variables)
---------------------------------------

Renderuje etykietę (label) dla przekazanego pola. Możesz opcjonalnie
jako drugi argument przekazać nazwę etykiety.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Dwie poniższe składanie są równoważne #}
    {{ form_label(form.name, 'Your Name', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Your name', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Renderuje każdy błąd dla danego pola.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# renderuje błędy globalne #}
    {{ form_errors(form) }}


form_widget(form.name, variables)
---------------------------------

Renderuje widget HTML dla przekazanego pola. Jeśli wykorzystasz ją
dla całego formularza lub kolekcji pól, każde z pól zostanie wyrenderowane.

.. code-block:: jinja

    {# render a widget, but add a "foo" class to it #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

Drugim argumentem funkcji ``form_widget`` jest tablica zmiennych.
Najbardziej powszechną zmienną jest ``attr``, która jest tablicą
atrybutów HTML które mają zostać zastosowane w widgecie.
W niektórych przypadkach, niektóre typy widgetów posiadają także 
inne opcje które mogą zostać przekazane. Zostały przedyskutowane 
w podstawach typ do typu.

form_row(form.name, variables)
------------------------------

Renderuje "rząd" dla przekazanego pola, który składa się z pól etykieta,
błędy oraz widget.

.. code-block:: jinja

    {# render a field row, but display a label with text "foo" #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

Drugim argumentem ``form_row`` jest tablica zmiennych. Szablony dostępne
w symfony pozwalają tylko nadpisać etykietę tak jak to jest pokazane
w przykładzie powyżej.

form_rest(form, variables)
--------------------------

Funkcja ta renderuje wszystkie pola które jeszcze nie zostały wyrenderowane
dla danego formularza. Dobrym pomysłem jest wykorzystanie tej funkcji w środku
formularza, ponieważ funkcja ta renderuje ukryte pola oraz pokazuje w oczywisty
sposób które pola zostały przez Ciebie pominięte (poprzez wyrenderowanie ich
dla Ciebie).

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Jeśli formularz posiada przynajmniej jedno pole z możliwością przesłania pliku,
funkcja ta wyrenderuje wymagany atrybut ``enctype="multipart/form-data"``.
Dobrym pomysłem jest aby zawsze używać tej funkcji w formularzu:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>
