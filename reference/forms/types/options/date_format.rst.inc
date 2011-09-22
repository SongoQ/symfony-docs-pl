format
~~~~~~

**typ**: ``integer`` lub ``string`` **domyślnie**: ``IntlDateFormatter::MEDIUM``

Opcja przekazywana do klasy `IntlDateFormatter``, która wykorzystywana jest 
do przekształcania danych wejściowych do odpowiedniego formatu. To jest ważne gdy opcja
`widgetu`_ jest ustawiona na ``single_text`` i określa jak przekształcić wejście. Domyślnie
format jest określany w oparciu o bierzące ustawienia lokalne użytkownika. Możesz przysłonić
go przekazując format jako ciąg znaków.
