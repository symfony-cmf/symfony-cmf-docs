.. index::
    single: current menu item; MenuBundle;

Current Menu Item Voters
========================

One of the aims of any menu system is to inform the user about where they are
in relation to the rest of the site. To do this the system needs to know which of
its menu items correspond to the page the user is currently viewing.

The KnpMenuBundle will identify a current menu item if the current request URI
matches the URI of the menu item.

This however only allows us to mark menu items as "current" where there is a
direct correspondence between the current request URI and the menu item URI.
In a CMS it is perhaps not desirable to have a menu item for each and every
unique page and it is perhaps preferable instead to indicate which menu item most
*closely corresponds* to the currently displayed page.

For example, you are developing a homepage application which features a blog.
The blog index page has a menu item but there are no menu items for the posts
associated with the blog and so none of the menu items will be highlighted. 

When the user is viewing a blog post you want the menu item for the blog to be
highlighted. The CMF facilitates this with current item *voter* classes.

The voter system allows you to register voter classes implementing
``VoterInterface``.  A voter class can "vote" for a menu item which it thinks
should be the current item. This vote, however, is not democratic - the first vote
that is cast will win.

.. note::

    The CMF MenuBundle is based on Knp Menu 1.x. The 2.0 rewrite of Knp Menu
    will add current item voters in the core Knp library. The CMF bundle
    voters are interface compatible and follow the same mechanism as Knp Menu
    to ease upgrading.

Voters
------

.. _bundles_menu_voters_request_identity_voter:

RequestContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter looks at the ``content`` field of the menu item extras and compares
it with the main content attribute of the request. The name for the main
content attribute in the request is configurable with the ``content_key``
option - if not set it defaults to the constant 
``Symfony\Cmf\Bundle\RoutingBundle\Routing\DynamicRouter::CONTENT_KEY`` (which resolves to ``contentDocument``).

You can enable this voter by adding the ``cmf_menu.voters.content_identity``
parameter to your configuration, note that the ``content_key`` parameter is
optional:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            voters:
                content_identity:
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
a ``Route`` instance associated with a menu item. When specified, this  will
cause the menu item to be marked as current if the current request URI begins
with, or matches, the ``currentUriPrefix`` option.

This method will enable an entire sub-path of a site to cause a menu item
to be marked as "current".

Imagine you have an articles section on your website. Articles are contained
in categories and categories can contain both articles and further categories.
There is a menu item "Articles" which has the URI ``/articles`` and there is
an article which can be reached at
``/articles/computers/laptops/thinkpad/X200``::

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuItem;
    use Acme\FooBundle\Document\Article;

    $dm = // get an instance of the document manager
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
    $article->setId('/articles/computers/laptops/thinkpad/X200');
    $article->setTitle('Thinkpad X200');
    // ...
    $dm->persist($article);

By associating the ``$articlesRoute`` with the ``MenuItem`` and setting the
``currentUriPrefix`` option to ``/articles`` the article "Thinkpad X200" will
cause the "Articles" menu item to be marked as current.

To enable the prefix voter, add the ``cmf_menu.voters.uri_prefix`` to your
configuration.

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            voters:
                uri-prefix: ~

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voters>
                    <uri-prefix />
                </voters>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'voters' => array(
                'uri_prefix' => null
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
content in the request and your model class to avoid calling getParent on
objects that do not have that method.  You need to tag the service as
``cmf_menu.voter`` and also as ``cmf_request_aware`` because it
depends on the request. The service looks the same as for complete custom
voters (see below), except you do not need to write your own PHP code:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_bundle.menu_voter.parent:
                class: Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter
                arguments:
                    - contentDocument
                    - "%my_bundle.my_model_class%"
                tags:
                    - { name: "cmf_menu.voter" }
                    - { name: "cmf_request_aware" }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <service id="my_bundle.menu_voter.parent"
                         class="Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter">
                    <argument>contentDocument</argument>
                    <argument>%my_bundle.my_model_class%</argument>
                    <tag name="cmf_menu.voter"/>
                    <tag name="cmf_request_aware"/>
                </service>
            </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(
            'Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter',
            array('contentDocument', '%my_bundle.my_model_class%')
        ));
        $definition->addTag('cmf_menu.voter');
        $definition->addTag('cmf_request_aware');

        $container->setDefinition('my_bundle.menu_voter.parent', $definition);

.. _bundles_menu_voters_custom_voter:

Custom Voter
------------

Voters must implement the ``Symfony\Cmf\MenuBundle\Voter\VoterInterface``. To
make the menu bundle notice the voter, tag it with ``cmf_menu.voter``.

If your voter requires the current ``Request`` object in its decision making
process you can add the :ref:`synchronized <bundle-core-tags-request-aware>`
tag to your service container definition. 

.. note::

    This tag is not required for versions of Symfony >= 2.3, see the
    :ref:`documentation <bundle-core-tags-request-aware>` for more information.

If you need to know the content the menu item points to, look in the
``content`` field of the menu item extras: ``$item->getExtra('content');``.
The ``ContentAwareFactory`` places the content referenced by the route there -
if it does reference a content. Your voter should handle the case where the
content is null.

For an example service definition see the section above for
``RequestParentIdentityVoter``.

A voter will look something like this::

    namespace Acme\MenuBundle\Voter;

    use Symfony\Cmf\Bundle\MenuBundle\Voter\VoterInterface;
    use Knp\Menu\ItemInterface;

    class MyVoter implements VoterInterface
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }

        /**
         * {@inheritDoc}
         */
        public function matchItem(ItemInterface $item)
        {
            if (...) {
                // $item is the current menu item
                return true;
            }
            if (...) {
                // $item for sure is NOT the current menu item
                // even if other voters might match
                return false;
            }

            // can't determine if this is the current menu item
            return null;
        }
    }

