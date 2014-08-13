.. index::
    single: Path Providers; RoutingAutoBundle
    
Token Providers
---------------

Token providers provide values for the tokens specified in the URI schemas.

content_method
~~~~~~~~~~~~~~

The ``content_method`` provider allows the content object (e.g. a forum
``Topic``) to specify a path using one of its methods. This is quite a powerful
method as it allows the content document to do whatever it can to produce the
route.

Options
.......

* ``method``: **required** Method used to return the route name/path/path elements.
* ``slugify``: If the return value should be slugified, default is ``true``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /my-forum/{title}
            token_providers:
                title: [content_method, {method: getTitle} ]

    .. code-block: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/my-forum/{title}">
                <token-provider token="category" name="content_method">
                    <option name="method">getCategoryName</option>
                </token-provider>
            </mapping>
        </auto-mapping>

This example will use the existing method "getTitle" of the ``Topic`` document
to retrieve the title. By default all strings are *slugified*.

Options
.......

* ``method``: **required** Method used to return the route name/path/path
  elements.
* ``slugify``: If the return value should be slugified, default is ``true``.

content_datetime
~~~~~~~~~~~~~~~~

The ``content_datetime`` provider will provide a path from a ``DateTime``
object provided by a designated method on the content document.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /blog/{date}/my-post
            token_providers:
                date: [content_datetime, {method: getDate} ]

    .. code-block: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/blog/{date}/my-post">
                <token-provider token="date" name="content_datetime">
                    <option name="method">getDate</option>
                </token-provider>
            </mapping>
        </auto-mapping>

Options
.......

* ``method``: **required** Method used to return the route name/path/path
  elements.
* ``slugify``: If the return value should be slugified, default is ``true``.
* ``date_format``: Any date format accepted by the `DateTime` class, default
  ``Y-m-d``.

content_locale
~~~~~~~~~~~~~~

The ``content_locale`` provider will provide the locale (e.g. ``fr``, ``de``,
etc) from the subject object. It ultimately determines the locale from the
storage specific adapter - so it is dependent upon the adapter supporting this
feature.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /blog/{locale}/my-post
            token_providers:
                locale: [content_locale ]

    .. code-block: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/blog/{locale}/my-post">
                <token-provider token="locale" name="content_locale" />
            </mapping>
        </auto-mapping>

Options
.......

* ``method``: **required** Method used to return the route name/path/path
  elements.
* ``slugify``: If the return value should be slugified, default is ``true``.
* ``locale_format``: Any locale format accepted by the `DateTime` class, default
  ``Y-m-d``.
