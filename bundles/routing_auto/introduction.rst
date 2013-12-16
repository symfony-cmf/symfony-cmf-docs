.. index::
    single: RoutingAuto; Bundles
    single: RoutingAutoBundle

RoutingAutoBundle
=================

.. include:: _not-stable-caution.rst.inc

    The RoutingAutoBundle allows you to define automatically created routes for
    documents.

This implies a separation of the ``Route`` and ``Content`` documents. If your
needs are simple this bundle may not be for you and you should have a look at
:doc:`the SimpleCmsBundle <simple_cms/introduction>`.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/routing-auto-bundle`_ package.

Features
--------

Imagine you are going to create a forum application that has two routeable
contents, a category and the topics for that forum. These documents are called
``Category`` and ``Topic``, and they are called *content documents*.

If you create a new category with the title "My New Category", the RoutingAutoBundle
automatically create the route ``/forum/my-new-cateogry``. For each new ``Topic``
it could create a route like ``/forum/my-new-category/my-topic-title``. This URL
resolves to a special type of route that is called an *auto route*.

By default, when you update a content document that has an auto route, the
corresponding auto route will also be updated. When deleting a content
document, the corresponding auto route will also be deleted.

If required, the bundle can also be configured to do extra stuff, like, for
example, leaving a ``RedirectRoute`` when the location of a content document
changes or automatically displaying an index page when an unconfigured
intermediate path is accessed (for example, listing all the children when requesting
``/forum`` instead of returning a ``404``).

Why not Simply Use a Single Route?
----------------------------------

Of course, our fictional forum application could use a single route with a
pattern ``/forum/my-new-forum/{topic}``, which could be handled by a controller.
Why not just do that?

#. By having a route for each page in the system, the application has a
   knowledge of which URLs are accessible. This can be very useful, for
   example, when specifying endpoints for menu items that are used when generating
   a site map;
#. By separating the route from the content you allow the route to be
   customized independently of the content, for example, a topic may have
   the same title as another topic but might need a different URL;
#. Separate route documents are translateable - this means you can have a URL
   for *each language*, "/welcome" and "/bienvenue" would each reference the
   same document in English and French respectively. This would be difficult
   if the slug was embedded in the content document;
#. By decoupling route and content the application doesn't care *what* is
   referenced in the route document. This means that you can easily replace the
   class of the document referenced.

Usage
-----

The diagram below shows a fictional URL for a forum topic. The first 6 elements
of the URL are called the *content path*. The last element is called the *content name*.

-- TODO update this image!!

.. image:: ../_images/bundles/routing_auto_post_schema.png

The content path is further broken down into *route stacks* and *routes*. A
route stack is a group of routes and routes are simply documents in the PHPCR
tree.

.. note::

    Although routes can be of any document class in this case, only objects
    which extend the :class:`Symfony\\Component\\Routing\\Route` object will
    be considered when matching a URL.

    The default behavior is to use ``Generic`` documents when generating a content
    path, and these documents will result in a 404 when accessed directly.

Internally, each route stack is built up by a *builder unit*. Builder units
contain one *path provider* class and two actions classes one action to take
if the provided path exists in the PHPCR tree, the other if it does not. The
goal of each builder unit is to generate a path and then provide a route
object for each element in that path.

The configuration for the example above could be as follows:

.. configuration-block::

    .. code-block:: yaml
    
        # app/config/config.yml
        cmf_routing_auto:
            mappings:

                Acme\ForumBundle\Document\Topic
                    content_path:
                        # corresponds first route stack in diagram: my-forum
                        forum_path:
                            provider: [specified, { path: my-form }]
                            exists_action: use
                            not_exists_action: create

                        # corresponds second route stack in diagram: my-category
                        category_path:
                            provider: [content_object, { method: getCategory }]
                            exists_action: use
                            not_exists_action: throw_exception

                        # corresponds third route stack in diagram: 2013/04/06
                        date:
                            provider: [content_datetime, { method: getPublishedDate } ]
                            exists_action: use
                            not_exists_action: create

                    # corresponds to the content name: my-topic-title
                    content_name:
                        provider: [content_method, { method: getTitle }]
                        exists_action: [auto_increment, { pattern: -%d }]
                        not_exists_action: create

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing_auto">

                <mapping class="Acme\ForumBundle\Document\Topic">

                    <content-path>
                        <!-- corresponds first route stack in diagram:  my-forum -->
                        <path-unit name="forum_path">
                            <provider name="specified">
                                <option name="path" value="my-forum" />
                            </provider>
                        </path-unit>

                        <!-- corresponds second route stack in diagram: my-category -->
                        <path-unit name="category_path">
                            <provider name="content_object">
                                <option name="method" value="getCategory" />
                            </provider>
                            <exists-action strategy="use" />
                            <not-exists-action strategy="throw_exception" />
                        </path-unit>

                        <!-- corresponds third route stack in diagram: 2013/04/06 -->
                        <path-unit name="date">
                            <provider name="content_datetime">
                                <option name="method" value="getPublishedDate" />
                            </provider>
                        </path-unit>
                    </content-path>


                    <!-- corresponds to the content name: my-topic-title -->
                    <content-name>
                        <provider name="content_method">
                            <option name="method" value="getTitle" />
                        </provider>
                        <exists-action strategy="auto_increment">
                            <option name="pattern" value="-%d" />
                        </exists-action>
                        <not-exists-action strategy="create" />
                    </content-name>
                </mapping>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing_auto', array(
            'mappings' => array(
                'Acme\ForumBundle\Document\Topic' => array(
                    'content_path' => array(
                        // corresponds first route stack in diagram: my-forum
                        'forum_path' => array(
                            'provider' => array('specified', array(
                                'path' => 'my-forum',
                            )),
                            'exists_action' => 'use',
                            'not_exists_action' => 'create',
                        ),

                        // corresponds second route stack in diagram: my-category
                        'category_path' => array(
                            'provider' => array('content_object', array(
                                'method' => 'getCategory',
                            )),
                            'exists_action' => 'use',
                            'not_exists_action' => 'throw_exception',
                        ),

                        // corresponds third route stack in diagram:  2013/04/06
                        'date' => array(
                            'provider' => array('content_datetime', array(
                                'method' => 'getPublishedDate',
                            )),
                            'exists_action' => 'use',
                            'not_exists_action' => 'create',
                        ),
                    ),

                    // corresponds to the content name: my-topic-title
                    'content_name' => array(
                        'provider' => array('content_method', array(
                            'method' => 'getTitle',
                        )),
                        'exists_action' => array('auto_increment', array(
                            'pattern' => '-%d',
                        )),
                        'not_exists_action' => 'create',
                    ),
                ),
            ),
        ));

The ``Topic`` document would then need to implement the methods named above as
follows::

    // src/Acme/ForumBundle/Document/Topic.php
    namespace Acme/ForumBundle/Document;

    class Topic
    {
        /**
         * Returns the category object associated with the topic.
         */
        public function getCategory()
        {
            return $this->category;
        }

        public function getPublishedDate()
        {
            return new \DateTime('2013/04/06');
        }

        public function getTitle()
        {
            return "My Topic Title";
        }
    }

After persisting this object, the route will be created. Of course, you need to make
the properties editable and then you have a fully working routing system.

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

Path Exists Actions
-------------------

These are the default actions available to take if the path provided by a
`path_provider` already exists and so creating a new path would create a
conflict.

auto_increment
~~~~~~~~~~~~~~

The ``auto_increment`` action will add a numerical suffix to the path, for
example ``my/path`` would first become ``my/path-1`` and if that path *also*
exists it will try ``my/path-2``, ``my/path-3`` and so on into infinity until
it finds a path which *doesn't* exist.

This action should typically be used in the ``content_name`` builder unit to
resolve conflicts. Using it in the ``content_path`` builder chain would not
make much sense (I can't imagine any use cases at the moment).

Example:

.. configuration-block::

    .. code-block:: yaml

        exists_action: auto_increment

    .. code-block:: xml

        <exists-action name="auto_increment" />

    .. code-block:: php

        array(
            // ...
            'exists_action' => 'auto_increment',
        );

use
~~~

The ``use`` action will simply take the existing path and use it. For example,
in our post example the first builder unit must first determine the blogs
path, ``/my/blog``, if this path exists (and it should) then we will *use* it
in the stack.

This action should typically be used in one of the content path builder units
to specify that we should use the existing route, on the other hand, using
this as the content name builder action should cause the old route to be
overwritten.

Example:

.. configuration-block::

    .. code-block:: yaml

        exists_action: use

    .. code-block:: xml

        <exists-action name="use" />

    .. code-block:: php

        array(
            // ...
            'exists_action' => 'use',
        );

Path not Exists Actions
-----------------------

These are the default actions available to take if the path provided by a
``path_provider`` does not exist.

create
~~~~~~

The ``create`` action will create the path. **currently** all routes provided
by the content path build units will be created as ``Generic`` documents,
whilst the content name route will be created as an ``AutoRoute`` document.

.. configuration-block::

    .. code-block:: yaml

        not_exists_action: create

    .. code-block:: xml

        <not-exists-action name="create" />

    .. code-block:: php

        array(
            // ...
            'not_exists_action' => 'create',
        );

throw_exception
~~~~~~~~~~~~~~~

This action will throw an exception if the route provided by the path provider
does not exist. You should take this action if you are sure that the route
*should* exist.

.. configuration-block::

    .. code-block:: yaml

        not_exists_action: throw_exception

    .. code-block:: xml

        <not-exists-action name="throw_exception" />

    .. code-block:: php

        array(
            // ...
            'not_exists_action' => 'throw_exception',
        );

Customization
-------------

.. _routingauto_customization_pathproviders:

Adding Path Providers
~~~~~~~~~~~~~~~~~~~~~

The goal of a ``PathProvider`` class is to add one or several path elements to
the route stack. For example, the following provider will add the path
``foo/bar`` to the route stack::

    <?php

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathProviderInterface;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class FoobarProvider implements PathProviderInterface
    {
        public function providePath(RouteStack $routeStack)
        {
            $routeStack->addPathElements(array('foo', 'bar'));
        }
    }

To use the path provider you must register it in the **DIC** and add the
``cmf_routing_auto.provider`` tag and set the **alias** accordingly.

.. configuration-block::

    .. code-block:: yaml

        my_cms.some_bundle.path_provider.foobar:
            class: "FoobarProvider"
            scope: prototype
            tags:
                - { name: cmf_routing_auto.provider, alias: "foobar"}

    .. code-block:: xml

        <service
            id="my_cms.some_bundle.path_provider.foobar"
            class="FoobarProvider"
            scope="prototype"
        >
            <tag name="cmf_routing_auto.provider" alias="foobar"/>
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('FooBarProvider');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'foobar'));
        $definition->setScope('prototype');

        $container->setDefinition('my_cms.some_bundle.path_provider.foobar', $definition);

The **foobar** path provider is now available as **foobar**.

.. note::

    The that both path providers and path actions need to be defined with a
    scope of "prototype". This ensures that each time the auto routing system
    requests the class a new one is given and we do not have any state
    problems.

Adding Path Actions
~~~~~~~~~~~~~~~~~~~

In the auto routing system, a "path action" is an action to take if the path
provided by the "path provider" exists or not.

You can add a path action by extending the ``PathActionInterface`` and
registering your new class correctly in the DI configuration.

This is a very simple implementation from the bundle - it is used to throw an
exception when a path already exists::

    namespace Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathNotExists;

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathActionInterface;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\Exception\CouldNotFindRouteException;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class ThrowException implements PathActionInterface
    {
        public function init(array $options)
        {
        }

        public function execute(RouteStack $routeStack)
        {
            throw new CouldNotFindRouteException('/'.$routeStack->getFullPath());
        }
    }

It is registered in the DI configuration as follows:

.. configuration-block::

    .. code-block:: yaml

        cmf_routing_auto.not_exists_action.throw_exception
            class: "My\Cms\AutoRoute\PathNotExists\ThrowException"
            scope: prototype
            tags:
                - { name: cmf_routing_auto.provider, alias: "throw_exception"}

    .. code-block:: xml

        <service
            id="my_cms.not_exists_action.throw_exception"
            class="My\Cms\AutoRoute\PathNotExists\ThrowException"
            scope="prototype"
            >
            <tag name="cmf_routing_auto.not_exists_action" alias="throw_exception"/>
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('My\Cms\AutoRoute\PathNotExists\ThrowException');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'throw_exception'));
        $definition->setScope('prototype');

        $container->setDefinition('my_cms.some_bundle.path_provider.throw_exception', $definition);

Note the following:

* **Scope**: Must *always* be set to *prototype*;
* **Tag**: The tag registers the service with the auto routing system, it can be one of the following;
    * ``cmf_routing_auto.exists.action`` - if the action is to be used when a path exists;
    * ``cmf_routing_auto.not_exists.action`` - if the action is to be used when a path does not exist;
* **Alias**: The alias of the tag is the name by which you will reference this action in the auto routing schema.

.. _`with composer`: http://getcomposer.org/
.. _`symfony-cmf/routing-auto-bundle`: https:/packagist.org/packages/symfony-cmf/routing-auto-bundle
