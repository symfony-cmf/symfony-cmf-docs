.. index::
    single: Path Providers; RoutingAutoBundle
    
Path Providers
--------------

Path providers specify a target path which is used by the subsequent path
actions to provide the actual route documents.

**Base** providers must be the first configured as the first builder in the
content path chain. This is because the paths that they provide correspond
directly to an existing path, i.e. they have an absolute reference.

specified (base provider)
~~~~~~~~~~~~~~~~~~~~~~~~~

This is the most basic path provider and allows you to specify an exact
(fixed) path.

Options
.......

* ``path`` - **required** The path to provide.

.. configuration-block::

    .. code-block:: yaml

        provider: [specified, { path: this/is/a/path }]

    .. code-block:: xml

        <provider name="specified">
            <option name="path" value="this/is/a/path" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('specified', array('path' => 'this/is/a/path')),
        );

.. caution::

    You should never specifiy absolute paths in the auto route system. If the
    builder unit is the first content path chain it is understood that it is
    the base of an absolute path.

content_object (base provider)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The content object provider will try and provide a path from an object
implementing ``RouteReferrersInterface`` provided by a designated method on the
content document. For example, if you have a ``Topic`` class, which has a
``getCategory`` method, using this provider you can tell the ``Topic`` auto route
to use the route of the category as a base.

So basically, if your category document has a path of ``/this/is/my/category``,
you can use this path as the base of your ``Category`` auto-route.

Options
.......

 - ``method``: **required** Method used to return the document whose route path you wish to use.

.. configuration-block::

    .. code-block:: yaml

        provider: [content_object, { method: getCategory }]

    .. code-block:: xml

        <provider name="content_object">
            <option name="method" value="getCategory" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('content_object', array('method' => 'getCategory')),
        );

.. note::

    At the time of writing translated objects are not supported. But a patch
    is already created for this feature.

content_method
~~~~~~~~~~~~~~

The ``content_method`` provider allows the content object (e.g. a forum
``Topic``) to specify a path using one of its methods. This is quite a powerful
method as it allows the content document to do whatever it can to produce the
route, the disadvantage is that your content document will have extra code in
it.

Options
.......

* ``method``: **required** Method used to return the route name/path/path elements.
* ``slugify``: If the return value should be slugified, default is ``true``.

.. configuration-block::

    .. code-block:: yaml

        provider: [content_method, { method: getTitle }]

    .. code-block:: xml

        <provider name="content_method">
            <option name="method" value="getTitle" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('content_method', array('method' => 'getTitle')),
        );

This example will use the existing method "getTitle" of the ``Topic`` document
to retrieve the title. By default all strings are *slugified*.

The method can return the path either as a single string, an array of path
elements or an object which can be converted into a string, as shown in the
following example::

    class Topic
    {
        /* Using a string */
        public function getTitle()
        {
            return "This is a topic";
        }

        /* Using an array */
        public function getPathElements()
        {
            return array('this', 'is', 'a', 'path');
        }

        /* Using an object */
        public function getStringObject()
        {
            $object = ...; // an object which has a __toString() method

            return $object;
        }
    }

content_datetime
~~~~~~~~~~~~~~~~

The ``content_datettime`` provider will provide a path from a ``DateTime``
object provided by a designated method on the content document.

.. configuration-block::

    .. code-block:: yaml

        provider: [content_datetime, { method: getDate, date_format: Y/m/d }]

    .. code-block:: xml

        <provider name="content_datetime">
            <option name="method" value="getDate" />
            <option name="date_format" value="Y/m/d" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('content_datetime', array(
                'method' => 'getDate',
                'date_format' => 'Y/m/d',
            )),
        );

.. note::

    This method extends `content_method`_ and inherits the slugify feature.
    Internally, it returns a string using the `DateTime->format()` method. This
    means that you can specify your date in anyway you like and it will be
    automatically slugified. Also, by adding path separators in the
    ``date_format`` you are effectively creating routes for each date component
    as slugify applies to **each element** of the path.

Options
.......

* ``method``: **required** Method used to return the route name/path/path
  elements.
* ``slugify``: If the return value should be slugified, default is ``true``.
* ``date_format``: Any date format accepted by the `DateTime` class, default
  ``Y-m-d``.
