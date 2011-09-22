input
~~~~~

**typ**: ``string`` **domyślnie**: ``datetime``

Format *wejściowy* danych - czyli format, w których data jest przechowywana
w Twoim obiekcie. Prawidłowe wartości to:

* ``string`` (e.g. ``2011-06-05``)
* ``datetime`` (a ``DateTime`` object)
* ``array`` (e.g. ``array('year' => 2011, 'month' => 06, 'day' => 05)``)
* ``timestamp`` (e.g. ``1307232000``)

Wartość zwracana z formularza, która będzie z powrotem znormalizowana
do tego formatu.