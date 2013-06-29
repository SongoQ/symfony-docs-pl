.. highlight:: php
   :linenothreshold: 2

.. index::
   single: formularze; pola; collection

Typ pola: collection
====================

Ten typ pola jest używany do renderowania "kolekcji" kilku pól lub formularzy.
W najprostrzej postaci może być to tablica pól typu ``text``, które wypełnia się
tablicą pól typu ``emails``. W bardziej skomplikowanym przykładzie, można osadzić
całe formularze, co jest użyteczne podczas tworzenia formularzy, które eksponują
relacje jeden-do-wielu (np. produktów, dla których zarządza się wieloma powiązanymi
fotografiami produktów).

+---------------+--------------------------------------------------------------------------+
| Renderowany   | zależy od opcji `type`_                                                  |
| jako          |                                                                          |
+---------------+--------------------------------------------------------------------------+
| Opcje         | - `type`_                                                                |
|               | - `options`_                                                             |
|               | - `allow_add`_                                                           |
|               | - `allow_delete`_                                                        |
|               | - `prototype`_                                                           |
|               | - `prototype_name`_                                                      |
+---------------+--------------------------------------------------------------------------+
| Opcje         | - `label`_                                                               |
| odziedziczone | - `error_bubbling`_                                                      |
|               | - `by_reference`_                                                        |
|               | - `empty_data`_                                                          |
|               | - `mapped`_                                                              |
+---------------+--------------------------------------------------------------------------+
| Typ nadrzędny | :doc:`form</reference/forms/types/form>`                                 |
+---------------+--------------------------------------------------------------------------+
| Klasa         | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CollectionType` |
+---------------+--------------------------------------------------------------------------+

.. note::

    Jeśli pracuje się z kolekcją encji Doctrine, trzeba zwrócić szczególną uwagę
    na opcje `allow_add`_, `allow_delete`_ i `by_reference`_. Proszę zobaczyć
    kompletny przykład w artykule :doc:`/cookbook/form/form_collections`.

Podstawowe użycie
-----------------

Typ ten używa się, gdy zachodzi potrzeba zarządzania kolekcją podobnych elementów
w formularzu. Na przykład załóżmy, że mamy pole ``emails`` korespondujące z tablicą
adresów email. W formularzu można pokazać każdy adres email w jego własnym polu
tekstowym::

    $builder->add('emails', 'collection', array(
        // each item in the array will be an "email" field
        'type'   => 'email',
        // these options are passed to each "email" type
        'options'  => array(
            'required'  => false,
            'attr'      => array('class' => 'email-box')
        ),
    ));

Najprościej jest wyrenderować wszystko na raz:

.. configuration-block::
   
    .. code-block:: jinja

        {{ form_row(form.emails) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']) ?>

Najbardziej elastyczny sposób wygląda tak:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {{ form_label(form.emails) }}
        {{ form_errors(form.emails) }}

        <ul>
        {% for emailField in form.emails %}
            <li>
                {{ form_errors(emailField) }}
                {{ form_widget(emailField) }}
            </li>
        {% endfor %}
        </ul>

    .. code-block:: html+php
       :linenos:

        <?php echo $view['form']->label($form['emails']) ?>
        <?php echo $view['form']->errors($form['emails']) ?>

        <ul>
        <?php foreach ($form['emails'] as $emailField): ?>
            <li>
                <?php echo $view['form']->errors($emailField) ?>
                <?php echo $view['form']->widget($emailField) ?>
            </li>
        <?php endforeach; ?>
        </ul>

W obu przypadkach nie zostanie wyrenderowane jakiekolwiek znacznik *input*, chyba
że tablica danych ``emails`` już zawiera jakieś adresy email.

W tym prostym przykładzie, jest jeszcze możliwe dodanie nowych lub usunięcie
istniejących adresów. Dodanie nowego adresu jest możliwe przez użycie opcji
`allow_add`_ (i ewentualnie opcji `prototype`_) (zobacza przykład poniżej).
Usunięcie adresów email z tablicy ``emails`` osiąga się poprzez użycie opcji
`allow_delete`_ .

Dodawanie i usuwanie elementów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli opcja `allow_add`_ jest ustawiona na ``true`` i gdy zgłoszone zostaną jakieś
nierozpoznane elementy, to zostaną one bezproblemowo dodane do tablicy elementów.
Jest to dobre w teorii, ale w praktyce wymaga trochę więcej wysiłku,  aby uzyskać
po stronie klienta prawidłowy kod JavaScript.

Kontynuując poprzedni przykład, załóżmy że, rozpoczynamy z dwoma adresami email
w tablicy ``emails``. W takim przypadku, zostana wyrenderowane dwa znaczniki input,
które będą wygladać, w zależności od nazwy formularza, mniej więcej tak:

.. code-block:: html
   :linenos:

    <input type="email" id="form_emails_0" name="form[emails][0]" value="foo@foo.com" />
    <input type="email" id="form_emails_1" name="form[emails][1]" value="bar@bar.com" />

Aby umożliwić użytkownikowi dodanie innego adresu email, wystarczy ustawić opcję
`allow_add`_ na ``true`` i (poprzez JavaScript) wyrenderować inne pole z nazwą
``form[emails][2]`` (i tak dale coraz więcej pól).

W celu ułatwienia, trzeba ustawić opcję `prototype`_ na ``true``, umożliwiając
wyrenderowanie pola "template", które można następnie użyć w kodzie JavaScript,
pomagając w ten sposób utworzyć to nowe pole. Wyrenderowane prototypowe pole
wygląda tak:

.. code-block:: html

    <input type="email" id="form_emails___name__" name="form[emails][__name__]" value="" />

Zastępując ``__name__`` jakąś unikalną wartością (np. ``2``), można zbudować i
dołączyć do formularza nowe pola HTML.

Przy wykorzystaniu jQuery prosty przykład może wyglądać, tak jak niżej. Jeżeli
renderujemy naraz kolekcję wszystkich pól (np. ``form_row(form.emails)``), to
następnie wszystkie rzeczy są nawet łatwiejsze, ponieważ atrybut ``data-prototype``
zostaje wyrenderowany automatycznie (z drobną różnicą – przeczytaj uwagę poniżej)
i wszystko, co się potrzebuje jest w kodzie JavaScript:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {{ form_start(form) }}
            {# ... #}

            {# przechowanie prototypu w atrybucie data-prototype #}
            <ul id="email-fields-list" data-prototype="{{ form_widget(form.emails.vars.prototype) | e }}">
            {% for emailField in form.emails %}
                <li>
                    {{ form_errors(emailField) }}
                    {{ form_widget(emailField) }}
                </li>
            {% endfor %}
            </ul>

            <a href="#" id="add-another-email">Add another email</a>

            {# ... #}
        {{ form_end(form) }}

        <script type="text/javascript">
            // śledzenie jak dużo pól email zostało wyrenderowanych
            var emailCount = '{{ form.emails | length }}';

            jQuery(document).ready(function() {
                jQuery('#add-another-email').click(function() {
                    var emailList = jQuery('#email-fields-list');

                    // pobranie szablonu prototype
                    var newWidget = emailList.attr('data-prototype');
                    // zamiana "__name__" używającego id i name prototypu
                    // na unikalną liczbę dla naszych emaili
                    // i zakończenie atrybutem name, wygladającym tak: name="contact[emails][2]"
                    newWidget = newWidget.replace(/__name__/g, emailCount);
                    emailCount++;

                    // utworzenie elementu listy i dodanie go do listy
                    var newLi = jQuery('<li></li>').html(newWidget);
                    newLi.appendTo(jQuery('#email-fields-list'));

                    return false;
                });
            })
        </script>

.. tip::

    Jeśli renderuje się na raz całą kolekcję, to prototyp jest dostępny automatycznie
    w atrybucie ``data-prototype`` elementu otaczającego kolekcję (np. ``div``
    lub ``table``). Jedyną różnicą jest to, że renderowany jest cały "wiersz formularza",
    co oznacza, że trzeba opakować go w jakiś element kontenerowy, tak jak to miało
    miejsce w powyższym przykładzie.

Opcje pola
----------

type
~~~~

**typ**: ``string`` lub :class:`Symfony\\Component\\Form\\FormTypeInterface` **wymagane**

Jest to typ pola dla każdego elementu w kolekcji (np. ``text``, ``choice`` itd.).
Na przykład, jeśli ma się tablicę adresów email, można użyć typu
:doc:`email</reference/forms/types/email>`. Jeśli chce się osadzić kolekcję w
jakimś innym formularzu, to trzeba utworzyć nową instancję typu formularzowego
i przekazać ten obiekt jako tą opcję.

options
~~~~~~~

**typ**: ``array`` **domyślnie**: ``array()``

Jest to tablica przekazywana do typ formularzowego określonego w opcji `type`_.
Na przykład, jeśli używa się typu :doc:`choice</reference/forms/types/choice>`
określonego w opcji `type`_ (np. dla kolekcji menu rozwijanego), to trzeba co
najmniej przekazać opcję ``choices`` do odpowiedniego typu::

    $builder->add('favorite_cities', 'collection', array(
        'type'   => 'choice',
        'options'  => array(
            'choices'  => array(
                'nashville' => 'Nashville',
                'paris'     => 'Paris',
                'berlin'    => 'Berlin',
                'london'    => 'London',
            ),
        ),
    ));

allow_add
~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``false``

Jeśli ustawiona na ``true``, to jeśli zgłoszone zostaną elementy nie ujęte w kolekcji,
to będą one dodane jako nowe elementy kolekcji. Ostateczna tablica będzie zawierać
istniejące elementy, jak też elementy nowe elementy. W celu poznania szczegółów
zobacz powyższy przykład.

Opcja `prototype`_ może zostać użyta do pomocy w wyrenderowaniu elementu protoptypu,
który można wykorzystać (w JavaScript) do dynamicznego tworzenia nowych elementów
formularza po stronie klienta. Więcej informacji można znaleźć w powyższym przykładzie
i artykule „:ref:`cookbook-form-collections-new-prototype`”.

.. caution::

    Jeśli osadza się w całości inne formularze w celu odzwierciedlenia relacji
    bazodanowej jeden-do-wielu, być może trzeba będzie ręcznie zapewnić klucz
    obcy tych nowych obiektów, by zostało to odpowiednio ustawione. Jeśli stosuje
    się Doctrine, to nie odbędzie się to automatycznie. W celu poznania szczegółów,
    proszę odwiedzić powyższy link.

allow_delete
~~~~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``false``

Jeśli jest ustawiona na ``true``, to jeśli istniejący element kolekcji nie zostanie
objety w zgłoszeniu danych formularza, to element ten nie zostanie dodany do elementów
ostatecznej tablicy. Oznacza to, że można zaimplementować przyciski "delete" w
JavaScript, który po prostu usunie elemnt ze struktury DOM. Gdy użytkownik wyśle
formularz, to brak tego elementu w zgłoszonych danych spowoduje, że zostanie ten
elemnt usunięty z ostatecznej tablicy.

Więcej informacji można znaleźć w artykule ":ref:`cookbook-form-collections-remove`".

.. caution::

    Należy zachować ostrożność gdy używa się tą opcję do osadzenia kolekcji obiektów.
    W takim przypadku, jeśli zostaną usunięte jakieś osadzone formularze, to będzie
    ich zwyczajnie brakować w ostatecznej tablicy obiektów. Jednakże, w zależności
    od logiki aplikacji, gdy usunięty ma być jeden z tych obiektów, to można go
    usunąć lub usunąć tylko klucz obcy odwołujący się do obiektu głównego.
    Nie jesto jednak obsługiwane automatycznie. W celu uzyskania więcej informacji
    proszę przeczytać artykuł „:ref:`cookbook-form-collections-remove`”.

prototype
~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``true``

Opcja ta jest przydatna, gdy wykorzystuje się opcję `allow_add`_. Jeśli ``true``
(i jeśli `allow_add`_ jest też ustawione na ``true``), to dostępny będzie specjalny
atrybut "prototype", tak że można wyrenderować jakiś przykładowy "szablon" na swojej
stronie, pokazujący jak powinien wyglądać nowy element. Podawany do tego elementu
atrybut ``name``, to ``__name__``. Umożliwia on dodanie poprzez JavaScript przycisku
w stylu "dodaj kolejny", który czyta prototyp, zamienia ``__name__`` na unikalną
nazwę lub liczbę i renderuje elemnt wewnątrz formularza. Podczas wysyłania formularza,
zostanie on dodany do podstawowej tablicy dzięki opcji `allow_add`_.

Pole prototypu może być renderowane poprzez zmienną ``prototype`` w polu typu *collection*:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails.vars.prototype) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']->vars['prototype']) ?>

Proszę zauwżyć, że wszystko, czego się potrzebuje, to "widget". Lecz w zależności
od tego jak renderuje się formularz, posługiwanie się całym "wierszem formularza"
może być łatwiejsze.

.. tip::

    Jeśli renderuje się na raz całe pole typu *collection*, to w atrybucie
    ``data-prototype`` elementu automatycznie pojawia się wiersz formularza
    z prototypem otaczający kolekcję (np. ``div`` lub ``table``).

Szczegóły tego jak można w rzeczywistości wykorzystać tą opcje pokazane są w powyższym
przykładzie jak również omówione są w artykule „:ref:`cookbook-form-collections-new-prototype`”.

prototype_name
~~~~~~~~~~~~~~

**typ**: ``String`` **domyślnie**: ``__name__``

Jeśli ma się w formularzu kilka kolekcji lub, co gorsza, kolekcje zagnieżdżone,
to można chcieć zmienić wieloznacznik, aby wieloznaczniki niepowiązane nie zostały
przekształcone na taką samą wartość.

Opcje odziedziczone
-------------------

Opcje dziedziczące z typu doc:`field</reference/forms/types/form>`.
Tutaj nie wymieniono wszystkich opcji – tylko te najbardziej wykorzystywane w tym
typie:

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

error_bubbling
~~~~~~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``true``

.. include:: /reference/forms/types/options/_error_bubbling_body.rst.inc

.. _reference-form-types-by-reference:

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
