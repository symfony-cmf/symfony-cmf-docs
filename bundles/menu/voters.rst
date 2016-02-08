.. index::
    single: current menu item; MenuBundle;

Current Menu Item Voters
========================

One of the aims of any menu system is to inform the user about where they are
in relation to the rest of the site. To do this the system needs to know which of
its menu items correspond to the page the user is currently viewing.

To determine which menu item should be marked as the current item, KnpMenu
includes voters. By default, KnpMenu comes with a voter that uses the Symfony
router to determine if the current page is equal to the page referenced by the
item.

This works well for static pages, highlighting the "About" item when viewing
the about page. However, in a dynamic CMS website, you want more flexibility.
For instance, each blog post does not have its own menu item, instead only the
index page listing the posts has an item. When viewing a blog post, you want
the listings page to be highlighted.

.. note::

    The voting process isn't using a democratic approach. Instead, the first
    voter to return either ``true`` or ``false`` is used to make the decision.

.. versionadded:: 2.0
    Voters were introduced in KnpMenu 2.0, which is first supported by
    CmfMenuBundle 2.0. Prior to version 2.0, the CmfMenuBundle provided voter
    support.

Provided Voters
---------------

.. _bundles_menu_voters_request_identity_voter:

RequestContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter looks at the ``content`` field of the menu item extras and compares
it with the main content attribute of the request. The name for the main
content attribute in the request is configurable with the ``content_key``
option - if not set it defaults to the constant
``Symfony\Cmf\Bundle\RoutingBundle\Routing\DynamicRouter::CONTENT_KEY`` (which resolves to ``contentDocument``).

You can enable this voter by adding the ``cmf_menu.voters.content_identity``
setting to your configuration. Example config:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            voters:
                content_identity:
                    enabled: true
                    content_key: myKey

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voters>
                    <content-identity content-key="myKey" />
                </voters>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'voters' => array(
                'content_identity' => array(
                    'enabled'     => true,
                    'content_key' => 'myKey',
                ),
            ),
        ));

.. note::

    This voter is not generally very useful - in most cases the route of the
    content document will match the route of the request URI anyway in which
    case this voter will not be called. An edge case might be if you modify the
    request within the controller: If you set the content in the request manually
    before rendering then any route associated with the content may not correspond
    with the current request URI. In such a case this voter will help.

.. _bundles_menu_voters_uri_prefix_voter:

UriPrefixVoter
~~~~~~~~~~~~~~

The ``UriPrefixVoter`` allows you to specify a ``currentUriPrefix`` option on
a ``Route`` instance associated with a menu item. When specified, this will
cause the menu item to be marked as current if the current request URI begins
with, or matches, the ``currentUriPrefix`` option.

This method will enable an entire sub-path of a site to cause a menu item
to be marked as "current".

Imagine you have a blog section on your website. Posts are contained in
categories and categories can contain both articles and further categories.
There is a menu item "Articles" which has the URI ``/articles`` and there is an
article which can be reached at ``/articles/computers/laptops/acme/A200``::

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuItem;
    use Acme\FooBundle\Document\Article;

    $dm = ...; // get an instance of the document manager

    $articlesRoute = new Route();
    // ...
    $articlesRoute->setId('/articles');
    $articlesRoute->setOption('currentUriPrefix', '/articles');

    $dm->persist($articlesRoute);

    $menuItem = new MenuItem();
    // ...
    $menuItem->setLabel('Articles');
    $menuItem->setContent($articlesRoute);

    $dm->persist($menuItem);

    $article = new Article();
    $article->setId('/articles/computers/laptops/acme/A200');
    $article->setTitle('Acme A200');
    // ...
    $dm->persist($article);

By associating the ``$articlesRoute`` with the ``MenuItem`` and setting the
``currentUriPrefix`` option to ``/articles`` the article "Acme A200" will cause
the "Articles" menu item to be marked as current.

To enable the prefix voter, add the ``cmf_menu.voters.uri_prefix`` to your
configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_menu:
            voters:
                uri-prefix: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voters>
                    <uri-prefix />
                </voters>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_menu', array(
            'voters' => array(
                'uri_prefix' => true,
            ),
        ));

RequestParentContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter is similar in concept to the
:ref:`bundles_menu_voters_request_identity_voter` but instead of comparing
request content with the menu item content, it compares the *parent* of the
request content with the menu item content.

Imagine you are creating a blogging platform. Each blog is represented by a
document in the PHPCR-ODM tree. The posts of the blog are the children of this
document. Each blog and each post is associated with a URI by way of an
associated route and the blog document is associated with a menu item:

.. code-block:: text

    cms/
        /blogs
            /my-blog (Route URI = /blog, Menu Item = "Blog")
                /my-first-post (Route URI = /blog/2013-10-02/my-first-post)
                /my-second-post (Route URI = /blog/2013-10-03/my-second-post)

This voter will enable you to make the "Blog" menu item "current" when viewing
a post - for example ``/blog/2013-10-02/my-first-post``. This is because the
parent of ``my-first-post`` is the blog document associated with the "Blog"
menu item.

To use this voter you need to configure a custom service with the name of the
content in the request and your model class to avoid calling ``getParent`` on
objects that do not have that method. You need to tag the service as
``knp_menu.voter``. The service looks the same as for complete custom
voters (see below), except you do not need to write your own PHP code:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            my_bundle.menu_voter.parent:
                class: Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter
                arguments:
                    - contentDocument
                    - '%my_bundle.my_model_class%'
                tags:
                    - { name: "knp_menu.voter", request: true }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_bundle.menu_voter.parent"
                         class="Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter">
                    <argument>contentDocument</argument>
                    <argument>%my_bundle.my_model_class%</argument>

                    <tag name="knp_menu.voter" request="true"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(
            'Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter',
            array('contentDocument', '%my_bundle.my_model_class%')
        ));
        $definition->addMethodCall('setRequest', array(
            new Reference(
                'request',
                ContainerInterface::NULL_ON_INVALID_REFERENCE,
                false
            )
        ));
        $definition->addTag('knp_menu.voter', array('request' => true));

        $container->setDefinition('my_bundle.menu_voter.parent', $definition);

.. _bundles_menu_voters_custom_voter:

Creating a Custom Voter
-----------------------

Voters must implement the ``Knp\MenuBundle\Matcher\Voter\VoterInterface``. To
make the menu bundle notice the voter, tag it with ``knp_menu.voter``.

If you need to know the content the menu item points to, look in the
``content`` field of the menu item extras: ``$item->getExtra('content');``.

A voter will look something like this::

    namespace AppBundle\Voter;

    use Knp\Menu\ItemInterface;
    use Knp\MenuBundle\Matcher\Voter\VoterInterface;

    class MyVoter implements VoterInterface
    {
        /**
         * {@inheritDoc}
         */
        public function matchItem(ItemInterface $item)
        {
            if ($this->isCurrent($item)) {
                return true;
            }

            if ($this->isSurelyNotCurrent($item)) {
                // $item for sure is NOT the current menu item
                // even if other voters might match
                return false;
            }

            // can't determine if this is the current menu item
            return null;
        }

        private function isCurrent(ItemInterface $item)
        {
            // ...
        }

        private function isSurelyNotCurrent(ItemInterface $item)
        {
            // ...
        }
    }
