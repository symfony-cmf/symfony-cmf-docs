.. index::
    single: Path Providers; RoutingAutoBundle
    
Path Providers
--------------

Path providers specify a target path which is used by the subsequent path
actions to provide the actual route documents.

**Base** providers must be the first configured as the first builder in the
content path chain.  This is because the paths that they provide correspond
directly to an existing path, i.e. they have an absolute reference.

specified (base provider)
~~~~~~~~~~~~~~~~~~~~~~~~~

This is the most basic path provider and allows you to specify an exact
(fixed) path.

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

Options:

* ``path`` - **required** The path to provide.

.. note::

    You never specifiy absolute paths in the auto route system. If the builder
    unit is the first content path chain it is understood that it is the base
    of an absolute path.

content_object (base provider)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The content object provider will try and provide a path from an object
implementing ``RouteReferrersInterface`` provided by a designated method on the
content document. For example, if you have a ``Post`` class, which has a
``getBlog`` method, using this provider you can tell the ``Post`` auto route
to use the route of the blog as a base.

So basically, if your blog content has a path of ``/this/is/my/blog`` you can
use this path as the base of your ``Post`` auto-route.

Example:

.. configuration-block::

    .. code-block:: yaml

        provider: [content_object, { method: getBlog }]

    .. code-block:: xml

        <provider name="content_object">
            <option name="method" value="getBlog" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('content_object', array('method' => 'getBlog')),
        );

.. note::

    At the time of writing translated objects are not supported. This isn't hard to do, but well, I just
    havn't done it yet.

Options:

 - ``method``: **required** Method used to return the document whose route path we wish to use.

content_method
~~~~~~~~~~~~~~

The ``content_method`` provider allows the content object (e.g. a blog
``Post``) to specify a path using one of its methods. This is quite a powerful
method as it allows the content document to do whatever it can to produce the
route, the disadvantage is that your content document will have extra code in
it.

**Example 1**:

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

This example will use the existing method "getTitle" of the ``Post`` document
to retrieve the title. By default all strings are *slugified*.

The method can return the path either as a single string or an array of path
elements as shown in the following example::

    class Post
    {
         public function getTitle()
         {
             return "This is a post";
         }

         public function getPathElements()
         {
             return array('this', 'is', 'a', 'path');
         }
    }

Options:

* ``method``: **required** Method used to return the route name/path/path elements.
* ``slugify``: If we should use the slugifier, default is ``true``.

content_datetime
~~~~~~~~~~~~~~~~

The ``content_datettime`` provider will provide a path from a ``DateTime``
object provided by a designated method on the content document.

**Example 1**:

.. configuration-block::

    .. code-block:: yaml

        provider: [content_datetime, { method: getDate }]

    .. code-block:: xml

        <provider name="content_datetime">
            <option name="method" value="getDate" />
        </provider>

    .. code-block:: php

        array(
            // ...
            'provider' => array('content_datetime', array('method' => 'getDate')),
        );

**Example 2**:

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

    This method extends `content_method` and inherits the slugify feature.
    Internally we return a string using the `DateTime->format()` method. This
    means that you can specify your date in anyway you like and it will be
    automatically slugified, also, by adding path separators in the
    `date_format` you are effectively creating routes for each date component
    as slugify applies to **each element** of the path.

Options:

* ``method``: **required** Method used to return the route name/path/path
  elements.
* ``slugify``: If we should use the slugifier, default is ``true``.
* ``date_format``: Any date format accepted by the `DateTime` class, default
  ``Y-m-d``.
