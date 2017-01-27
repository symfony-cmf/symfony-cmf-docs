.. index::
    single: Menus; SimpleCmsBundle

Menus
-----

.. include:: ../_partials/unmaintained.rst.inc

You can use `Knp Menu Bundle`_ to render a menu of your SimpleCms pages. The default Page document
(``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page``) implements the ``Knp\Menu\NodeInterface``
which allows for rendering them as a menu.

.. configuration-block::

    .. code-block:: jinja

        {{ knp_menu_render('/cms/simple/mypage') }}

    .. code-block:: html+php

        <?php echo $view['knp_menu']->render('/cms/simple/mypage') ?>

Menu options can be customized for each `Page` using the following public methods of the `Page`.

``setAttributes(array $attributes)``, ``setAttribute($name, $value)``
    Set one or more html attributes to be used when rendering the item (generally the ``<li>`` tag)

``setLabel($label)``
    Set the label text to be used

``setLabelAttributes($labelAttributes)``
    Set html attributes to be used when rendering the label

``setChildrenAttributes(array $attributes)``
    Set one or more html attributes to be used on the element containing the children (generally the ``<ul>`` tag)

``setLinkAttributes($linkAttributes)``
    Set html attributes to be used when rendering the link tag

``setDisplay($display)``
    Boolean which determine if the page should be included in menus

``setDisplayChildren($displayChildren)``
    Boolean which determines whether children should be added to the menu

.. tip::

    If you use Sonata Admin in your project you can edit the menu options
    using the MenuOptionsExtension that comes with the menu bundle. For more
    information on how to use it take a look at the :doc:`menu bundle documentation <../menu/sonata_admin>`

.. _`Knp Menu Bundle`: https://github.com/KnpLabs/KnpMenuBundle
