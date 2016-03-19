.. index::
    single: Path Providers; RoutingAutoBundle
    
Token Providers
===============

Token providers provide values for the tokens specified in the URI schemas.
Such values can be derived form the object for which the route is being
generated or from the environment (e.g. you could use the current locale in
the route).

Global Options
--------------

The following options apply to all token providers:

``allow_empty``
    Allow the token value to be empty. If the token value is empty then any
    trailing slash will be removed. If the token is the last token, then the
    leading slash will be removed. Default is ``false``.

``content_method``
------------------

The ``content_method`` provider allows the content object (e.g. a forum
``Topic``) to specify a path using one of its methods. This is quite a powerful
method as it allows the content document to do whatever it can to produce the
route.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /my-forum/{title}
            token_providers:
                title: [content_method, { method: getTitle }]

    .. code-block:: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/my-forum/{title}">
                <token-provider token="category" name="content_method">
                    <option name="method">getCategoryName</option>
                </token-provider>
            </mapping>
        </auto-mapping>

This example will use the existing method ``getTitle`` of the ``Topic`` document
to retrieve the title. By default all strings are *slugified*.

Options
~~~~~~~

``method``
    **required** Method used to return the route name/path/path elements.
``slugify``
    If the return value should be slugified, default is ``true``.

``content_datetime``
--------------------

The ``content_datetime`` provider will provide a path from a ``DateTime``
object provided by a designated method on the content document.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /blog/{date}/my-post
            token_providers:
                date: [content_datetime, { method: getDate }]

    .. code-block: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/blog/{date}/my-post">
                <token-provider token="date" name="content_datetime">
                    <option name="method">getDate</option>
                </token-provider>
            </mapping>
        </auto-mapping>

Options
~~~~~~~

``method``
    **required** Method used to return the route name/path/path elements.
``slugify``
    If the return value should be slugified, default is ``true``.
``date_format``
    Any date format accepted by the `DateTime` class, default ``Y-m-d``.

``content_locale``
------------------

The ``content_locale`` provider will provide the locale (e.g. ``fr``, ``de``,
etc) from the subject object. It ultimately determines the locale from the
storage specific adapter - so it is dependent upon the adapter supporting this
feature.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            uri_schema: /blog/{locale}/my-post
            token_providers:
                locale: [content_locale]

    .. code-block: xml

        <?xml version="1.0" ?>
        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic" uri-schema="/blog/{locale}/my-post">
                <token-provider token="locale" name="content_locale" />
            </mapping>
        </auto-mapping>

``container``
-------------

.. versionadded:: 1.1
    The container provider was introduced in RoutingAutoBundle 1.1

The ``container`` provider allows you to use parameters which have
been defined in the Symfony DI container.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Article:
            uri_schema: {base_url}/good-day
            token_providers:
                base_url: [container, { parameter: my_parameter.base_path }

    .. code-block: xml

        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <?xml version="1.0" ?>
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Article" uri-schema="/{base_url}/good-day">
                <token-provider token="base_url" name="container" >
                    <option name="parameter">my_parameter.base_path</option>
                </token>
            </mapping>
        </auto-mapping>

.. note::

    Parameters from the container will not be slugified. It is your
    responsibility to ensure that they contain safe characters.

Options
~~~~~~~

This token provider has no options.


Creating a Custom Token Provider
--------------------------------

To create a custom token provider, you have to create a class which implements
``TokenProviderInterface``. This class requires a method called ``provideValue()``
which returns the value of the token. It has access to the ``UriContext``,
which contains the current uri (``getUri()``), the subject object
(``getSubjectObject()``), the locale (``getLocale()``) and the auto route
(``getAutoRoute()``).

The class also requires a method called ``configureOptions()``. This method can
configure any options using the `OptionsResolver component`_.

The following token provider doesn't have any options and simply always returns
``'foobar'``::

    // src/Acme/CmsBundle/RoutingAuto/PathProvider/FoobarTokenProvider.php
    namespace Symfony\Cmf\Component\RoutingAuto\TokenProvider;

    use Symfony\Cmf\Component\RoutingAuto\TokenProviderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Symfony\Cmf\Component\RoutingAuto\UriContext;

    class FoobarTokenProvider implements TokenProviderInterface
    {
        /**
         * {@inheritDoc}
         */
        public function provideValue(UriContext $uriContext, $options)
        {
            return 'foobar';
        }

        /**
         * {@inheritDoc}
         */
        public function configureOptions(OptionsResolverInterface $optionsResolver)
        {
        }
    }

To use the path provider, you must register it in the container and add the
``cmf_routing_auto.token_provider`` tag and set the **alias** accordingly:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_cms.token_provider.foobar:
                class: Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider
                tags:
                    - { name: cmf_routing_auto.token_provider, alias: "foobar" }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="acme_cms.token_provider.foobar"
                class="Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider"
            >
                <tag name="cmf_routing_auto.token_provider" alias="foobar"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider');
        $definition->addTag('cmf_routing_auto.token_provider', array('alias' => 'foobar'));

        $container->setDefinition('acme_cms.token_provider.foobar', $definition);

The ``FoobarTokenProvider`` is now available as **foobar** in the routing auto
configuration.

.. _`OptionsResolver component`: https://symfony.com/doc/current/components/options_resolver.html
