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

To help make this easier, setting the `prototype`_ option to ``true`` allows
you to render a "template" field, which you can then use in your JavaScript
to help you dynamically create these new fields. A rendered prototype field
will look like this:

.. code-block:: html

    <input type="email" id="form_emails___name__" name="form[emails][__name__]" value="" />

By replacing ``__name__`` with some unique value (e.g. ``2``),
you can build and insert new HTML fields into your form.

Using jQuery, a simple example might look like this. If you're rendering
your collection fields all at once (e.g. ``form_row(form.emails)``), then
things are even easier because the ``data-prototype`` attribute is rendered
automatically for you (with a slight difference - see note below) and all
you need is the JavaScript:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {# ... #}

            {# store the prototype on the data-prototype attribute #}
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
            // keep track of how many email fields have been rendered
            var emailCount = '{{ form.emails | length }}';

            jQuery(document).ready(function() {
                jQuery('#add-another-email').click(function() {
                    var emailList = jQuery('#email-fields-list');

                    // grab the prototype template
                    var newWidget = emailList.attr('data-prototype');
                    // replace the "__name__" used in the id and name of the prototype
                    // with a number that's unique to your emails
                    // end name attribute looks like name="contact[emails][2]"
                    newWidget = newWidget.replace(/__name__/g, emailCount);
                    emailCount++;

                    // create a new list element and add it to the list
                    var newLi = jQuery('<li></li>').html(newWidget);
                    newLi.appendTo(jQuery('#email-fields-list'));

                    return false;
                });
            })
        </script>

.. tip::

    If you're rendering the entire collection at once, then the prototype
    is automatically available on the ``data-prototype`` attribute of the
    element (e.g. ``div`` or ``table``) that surrounds your collection. The
    only difference is that the entire "form row" is rendered for you, meaning
    you wouldn't have to wrap it in any container element as was done
    above.

Field Options
-------------

type
~~~~

**type**: ``string`` or :class:`Symfony\\Component\\Form\\FormTypeInterface` **required**

This is the field type for each item in this collection (e.g. ``text``, ``choice``,
etc). For example, if you have an array of email addresses, you'd use the
:doc:`email</reference/forms/types/email>` type. If you want to embed
a collection of some other form, create a new instance of your form type
and pass it as this option.

options
~~~~~~~

**type**: ``array`` **default**: ``array()``

This is the array that's passed to the form type specified in the `type`_
option. For example, if you used the :doc:`choice</reference/forms/types/choice>`
type as your `type`_ option (e.g. for a collection of drop-down menus), then
you'd need to at least pass the ``choices`` option to the underlying type::

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

**type**: ``Boolean`` **default**: ``false``

If set to ``true``, then if unrecognized items are submitted to the collection,
they will be added as new items. The ending array will contain the existing
items as well as the new item that was in the submitted data. See the above
example for more details.

The `prototype`_ option can be used to help render a prototype item that
can be used - with JavaScript - to create new form items dynamically on the
client side. For more information, see the above example and :ref:`cookbook-form-collections-new-prototype`.

.. caution::

    If you're embedding entire other forms to reflect a one-to-many database
    relationship, you may need to manually ensure that the foreign key of
    these new objects is set correctly. If you're using Doctrine, this won't
    happen automatically. See the above link for more details.

allow_delete
~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``false``

If set to ``true``, then if an existing item is not contained in the submitted
data, it will be correctly absent from the final array of items. This means
that you can implement a "delete" button via JavaScript which simply removes
a form element from the DOM. When the user submits the form, its absence
from the submitted data will mean that it's removed from the final array.

For more information, see :ref:`cookbook-form-collections-remove`.

.. caution::

    Be careful when using this option when you're embedding a collection
    of objects. In this case, if any embedded forms are removed, they *will*
    correctly be missing from the final array of objects. However, depending on
    your application logic, when one of those objects is removed, you may want
    to delete it or at least remove its foreign key reference to the main object.
    None of this is handled automatically. For more information, see
    :ref:`cookbook-form-collections-remove`.

prototype
~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

This option is useful when using the `allow_add`_ option. If ``true`` (and
if `allow_add`_ is also ``true``), a special "prototype" attribute will be
available so that you can render a "template" example on your page of what
a new element should look like. The ``name`` attribute given to this element
is ``__name__``. This allows you to add a "add another" button via JavaScript
which reads the prototype, replaces ``__name__`` with some unique name or
number, and render it inside your form. When submitted, it will be added
to your underlying array due to the `allow_add`_ option.

The prototype field can be rendered via the ``prototype`` variable in the
collection field:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails.vars.prototype) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']->vars['prototype']) ?>

Note that all you really need is the "widget", but depending on how you're
rendering your form, having the entire "form row" may be easier for you.

.. tip::

    If you're rendering the entire collection field at once, then the prototype
    form row is automatically available on the ``data-prototype`` attribute
    of the element (e.g. ``div`` or ``table``) that surrounds your collection.

For details on how to actually use this option, see the above example as well
as :ref:`cookbook-form-collections-new-prototype`.

prototype_name
~~~~~~~~~~~~~~

**type**: ``String`` **default**: ``__name__``

If you have several collections in your form, or worse, nested collections
you may want to change the placeholder so that unrelated placeholders are not
replaced with the same value.

Inherited options
-----------------

These options inherit from the :doc:`field</reference/forms/types/form>` type.
Not all options are listed here - only the most applicable to this type:

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

error_bubbling
~~~~~~~~~~~~~~

**type**: ``Boolean`` **default**: ``true``

.. include:: /reference/forms/types/options/_error_bubbling_body.rst.inc

.. _reference-form-types-by-reference:

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
