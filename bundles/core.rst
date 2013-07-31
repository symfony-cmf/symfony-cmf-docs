.. index::
    single: Core; Bundles
    single: CoreBundle

CoreBundle
==========

This is the `CoreBundle`_ for the Symfony2 content management framework. This
bundle provides common functionality, helpers and utilities for the other CMF
bundles.

One of the provided features is an interface and implementation of a publish
workflow checker with an accompanying interface that models can implement that
want to support this checker.

Furthermore it provides a twig helper exposing several useful functions for
twig templates to interact with PHPCR-ODM documents.

.. index:: CoreBundle, PHPCR, ODM, publish workflow

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            document_manager_name: ~ # used for the twig functions to fetch documents
            publish_workflow:
                enabled: true
                view_non_published_role: ROLE_CAN_VIEW_NON_PUBLISHED
                request_listener: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/core"
            document-manager-name="null">
            <publish-workflow
                enabled="true"
                view-non-published-role="ROLE_CAN_VIEW_NON_PUBLISHED"
                request-listener="true"
            />
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', array(
            'document_manager_name' => null,
            'publish_workflow'      => array(
                'enabled'                 => true,
                'view_non_published_role' => 'ROLE_CAN_VIEW_NON_PUBLISHED',
                'request_listener'        => true,
            ),
        ));

The publish workflow is enabled by default. If you do not want to use it, you
can set ``cmf_core.publish_workflow.enabled: false`` to gain some performance.

.. _bundle-core-publish_workflow:

Publish Workflow
----------------

The publish workflow system allows to control what content is available on the
site. This is similar to the `Symfony2 Security component`_. But contrary to the
security context, the publish check can be executed even when no firewall is in
place and the security context thus has no token (see `Symfony2 Authorization`_).

The publish workflow is also tied into the security workflow: The core bundle
registers a security voter that forwards security checks to the publish
workflow. This means that if you always have a firewall, you can just use
the normal security context and the twig function ``is_granted`` to check for
publication.

A good introduction to the Symfony core security is the `Security Chapter`_ in
the Symfony2 book.

Check if Content is Published
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Bundle provides the ``cmf_core.publish_workflow.checker`` service which
implements the :class:`Symfony\\Component\\Security\\Core\\SecurityContextInterface`
of the Symfony security component. The method to check publication is, like
with the security context,
:method:`Symfony\\Component\\Security\\Core\\SecurityContextInterface::isGranted`.

This method is used as when doing `ACL checks`_: The first argument is the
desired action, the second the content object you want to do the action on.

In 1.0, the only actions supported by the default voters are ``VIEW`` and
``VIEW_ANONYMOUS``. Having the right to view means that the current user is
allowed to see this content either because it is published or because of his
specific permissions. In some contexts, your application might not want to
show unpublished content even to a privileged user so as not to confuse him.
For this, the "view anonymous" permission is used.

The workflow checker is configured with a role that is allowed to bypass
publication checks so that it can see unpublished content. This role should be
given to editors. The default name of the role is ``ROLE_CAN_VIEW_NON_PUBLISHED``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            role_hierarchy:
                ROLE_EDITOR:       ROLE_CAN_VIEW_NON_PUBLISHED

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config xmlns="http://symfony.com/schema/dic/security">
            <role id="ROLE_EDITOR">ROLE_CAN_VIEW_NON_PUBLISHED</role>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'role_hierarchy' => array(
                'ROLE_EDITOR' => 'ROLE_CAN_VIEW_NON_PUBLISHED',
            ),
        ));

Once a user with ``ROLE_EDITOR`` is logged in - meaning there is a firewall in place for the path
in question - he will have the permission to view unpublished content as well::

    use Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishWorkflowChecker;

    // check if current user is allowed to see this document
    $publishWorkflowChecker = $container->get('cmf_core.publish_workflow.checker');
    if ($publishWorkflowChecker->isGranted(
        PublishWorkflowChecker::VIEW_ATTRIBUTE,
        $document)
    ) {
        // ...
    }
    // check if the document is published. even if the current role would allow
    // to see the document, this will still return false if the documet is not
    // published
    if ($publishWorkflowChecker->isGranted(
        PublishWorkflowChecker::VIEW_ANONYMOUS_ATTRIBUTE,
        $document
    )) {
        // ...
    }

.. _bundle-core-publish_workflow-twig_function:

To check publication in a template, use the twig function ``cmf_is_published``:

.. configuration-block::

    .. code-block:: jinja

        {# check if document is published, regardless of current users role #}
        {% if cmf_is_published(page) %}
            {# ... output the document #}
        {% endif %}

        {#
            check if current logged in user is allowed to view the document either
            because it is published or because the current user may view unpublished
            documents.
        #}
        {% if is_granted('VIEW', page) %}
            {# ... output the document #}
        {% endif %}

    .. code-block:: php

        <!-- check if document is published, regardless of current users role -->
        <?php if ($view['cmf']->isPublished($page)) : ?>
            <!-- ... output the document -->
        <?php endif ?>

        <!--
            check if current logged in user is allowed to view the document either
            because it is published or because the current user may view unpublished
            documents.
        -->
        <?php if ($view['security']->isGranted('VIEW', $page)) : ?>
            <!-- ... output the document -->
        <?php endif ?>

Code that loads content should do the publish checks. Note that the twig
functions already check for publication. Thanks to a
:ref:`request listener <bundle-core-workflow-request_listener>`, routes and
the main content provided by the
:ref:`DynamicRouter <bundles-routing-dynamic_router>` are checked automatically
as well.

It is possible to set the security token explicitly on the workflow checker.
But by default, the checker will acquire the token from the default security
context, and if there is none (typically when there is no firewall in place for
that URL), an
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AnonymousToken`
is created on the fly.

If you check for ``VIEW`` and not ``VIEW_ANONYMOUS``, the first check is
whether the security context knows the current user and if that user is granted
the bypass role. If so, access is granted, otherwise the decision is delegated to a
:class:`Symfony\\Component\\Security\\Core\\Authorization\\AccessDecisionManager`
which calls all voters with the requested attributes, the object and the token.

The decision manager is configured for an unanimous vote with "allow if all
abstain". This means a single voter saying ``ACCESS_DENIED`` is enough for
the content to be considered not published. If all voters abstain (for example
when the content in question does not implement any workflow features) the
content is still considered published.

Publish Voters
~~~~~~~~~~~~~~

A voter has to implement the
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`.
It will get passed a content object and has to decide whether it is published
according to its rules. The CoreBundle provides a couple of generic voters
that check the content for having an interface exposing the methods they need.
If the content implements the interface, they check the parameter and return
``ACCESS_GRANTED`` or ``ACCESS_DENIED``, otherwise they return
``ACCESS_ABSTAIN``.

As voting is unanimous, each voter returns ``ACCESS_GRANTED`` if its criteria
is met, but if a single voter returns ``ACCESS_DENIED``, the content is
considered not published.

You can also implement your :ref:`own voters <bundle-core-workflow_custom_voters>`
for additional publication behaviour.

PublishableVoter
................

This voter checks on the ``PublishableInterface`` which simply has a method to
return a boolean value.

* **isPublishable**: If the object should be considered for publication or not.

TimePeriodVoter
...............

This voter checks on the ``PublishTimePeriodInterface`` which defines a start
and end date. A date may be null to indicate "always started" resp.
"never ending".

* **getPublishStartDate**: If non-null, the date from which the document
  should start being published;
* **getPublishEndDate**: If non-null, the date from which the document
  should stop being published.

.. _bundle-core-workflow_custom_voters:

Custom Voters
.............

To build voters with custom logic, you need to implement
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Voter\\VoterInterface`
and define a service with the tag ``cmf_published_voter``. This is similar
to the ``security.voter`` tag, but adds your voter to the publish workflow. As
with the security voters, you can specify a priority, though it is of limited
use as the access decision must be unanimous. If you have more expensive checks,
you can lower the priority of those voters.

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.security.publishable_voter:
                class: %my_namespace.security.publishable_voter.class%
                tags:
                    - { name: cmf_published_voter, priority: 30 }

    .. code-block:: xml

        <service id="acme.security.publishable_voter" class="%acme.security.publishable_voter.class%">
            <tag name="cmf_published_voter" priority="30"/>
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->register(
                'acme.security.publishable_voter',
                '%acme.security.publishable_voter.class%'
            )
            ->addTag('cmf_published_voter', array('priority' => 30))
        ;

As the workflow checker will create an
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AnonymousToken` on
the fly if the security context has none, voters must be able to handle this
situation when accessing the user. Also when accessing the security context,
they first must check if it has a token and otherwise not call it to avoid
triggering an exception. If a voter only gives access if there is a current
user fulfills some requirement, it simply has to return ``ACCESS_DENIED`` if
there is no current user.

.. _bundle-core-workflow-request_listener:

Publication Request Listener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :ref:`DynamicRouter <bundles-routing-dynamic_router>` places the route
object and the main content - if the route has a main content - into the
request attributes. Unless you disable the
``cmf_core.publish_workflow.request_listener``, this listener will listen
on all requests and check publication of both the route object and the main
content object.

This means that custom templates for ``templates_by_class`` and the controllers
of ``controllers_by_class`` need not check for publication explicitly as its
already done.

Editing publication information: Publish Workflow Sonata Admin Extension
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is a write interface for each publish workflow too, defining setter
methods. The core bundle provides extensions for SonataAdminBundle to easily
add editing of the publish workflow fields to all or selected admins.

Instead of implementing ``PublishableInterface`` resp.
``PublishTimePeriodInterface`` you models instead need to implement the
``PublishableWriteInterface`` and / or ``PublishTimePeriodWriteInterface``.

To enable the extensions in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                symfony_cmf_core.publish_workflow.admin_extension.publishable:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishableWriteInterface
                symfony_cmf_core.publish_workflow.admin_extension.time_period:
                    implements:
                        - Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishTimePeriodWriteInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://sonata-project.org/schema/dic/admin">
            <!-- ... -->
            <extension id="symfony_cmf_core.publish_workflow.admin_extension.publishable">
                <implement>
                    Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishableWriteInterface
                </implement>
            </extension>
            <extension id="symfony_cmf_core.publish_workflow.admin_extension.time_period">
                <implement>
                    Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishTimePeriodWriteInterface
               </implement>
           </extension>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'symfony_cmf_core.admin_extension.publish_workflow' => array(
                    'implements' => array(
                        'Symfony\Cmf\Bundle\CoreBundle\PublishWorkflow\PublishWorkflowInterface',
                    ),
                ),
            ),
        ));

See the `Sonata Admin extension documentation`_ for more information.

Dependency Injection Tags
-------------------------

cmf_request_aware
~~~~~~~~~~~~~~~~~

If you have services that need the request (e.g. for the publishing workflow
or current menu item voters), you can tag them with ``cmf_request_aware`` to
have a kernel listener inject the request. Any class used in such a tagged
service must have the ``setRequest`` method or you will get a fatal error::

    use Symfony\Component\HttpFoundation\Request;

    class MyClass
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }
    }

.. note::

    You should only use this tag on services that will be needed on every
    request. If you use this tag excessively you will run into performance
    issues. For seldom used services, you can inject the container in the
    service definition and call ``$this->container->get('request')`` in your
    code when you actually need the request.

For Symfony 2.3, this tag is automatically translated to a
`synchronized service`_ but as Symfony 2.2 does not have that feature, you can
use this tag for bundles that you want to be able to use with Symfony 2.2. In
custom applications that run with Symfony 2.3, there is no need for this tag,
just use the synchronized service feature.

cmf_published_voter
~~~~~~~~~~~~~~~~~~~

Used to activate :ref:`custom voters <bundle-core-workflow_custom_voters>` for the
:ref:`publish workflow <bundle-core-publish_workflow>` . Tagging a service with
``cmf_published_voter`` integrates it into the access decision of the publish
workflow.

This tag has the attribute *priority*. The lower the priority number, the
earlier the voter gets to vote.

Templating
----------

Twig
~~~~

<<<<<<< HEAD
The core bundle contains a Twig extension that provides a set of useful
functions for your templates. The functions respect the
:ref:`publish workflow <bundle-core-publish_workflow>` if it is

* **cmf_find**: returns the document for the provided path
* **cmf_find_many**: returns an array of documents for the provided paths
* **cmf_is_published**: checks if the provided document is published, see
  :ref:`bundle-core-publish_workflow-twig_function`.
* **cmf_prev**: returns the previous document by examining the child nodes of
  the provided parent
* **cmf_prev_linkable**: returns the previous linkable document by examining
  the child nodes of the provided parent
* **cmf_next**: returns the next document by examining the child nodes of the
  provided parent
* **cmf_next_linkable**: returns the next linkable document by examining the
  child nodes of the provided parent
* **cmf_child**: returns a child documents of the provided parent document and
  child node
* **cmf_children**: returns an array of all the children documents of the
  provided parent
* **cmf_linkable_children**: returns an array of all the linkable children
  documents of the provided parent
* **cmf_descendants**: returns an array of all descendants paths of the
  provided parent
* **cmf_document_locales**: gets the locales of the provided document
* **cmf_nodename**: returns the node name of the provided document
* **cmf_parent_path**: returns the parent path of the provided document
* **cmf_path**: returns the path of the provided document

.. code-block:: jinja

    {% set page = cmf_find('/some/path') %}

    {% if cmf_is_published(page) %}
        {% set prev = cmf_prev(page) %}
        {% if prev %}
            <a href="{{ path(prev) }}">prev</a>
        {% endif %}

        {% set next = cmf_next(page) %}
        {% if next %}
            <span style="float: right; padding-right: 40px;"><a href="{{ path(next) }}">next</a></span>
        {%  endif %}

        {% for news in cmf_children(page)|reverse %}
            <li><a href="{{ path(news) }}">{{ news.title }}</a> ({{ news.publishStartDate | date('Y-m-d')  }})</li>
        {% endfor %}

        {% if 'de' in cmf_document_locales(page) %}
            <a href="{{ path(
                app.request.attributes.get('_route'),
                app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({
                    '_locale': 'de'
                })
            ) }}">DE</a>
        {%  endif %}
        {% if 'fr' in cmf_document_locales(page) %}
            <a href="{{ path(
                app.request.attributes.get('_route'),
                app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({
                    '_locale': 'fr'
                })
            ) }}">FR</a>
        {% endif %}
    {% endif %}

PHP
~~~

The bundle also provides a templating helper to use in PHP templates, it
contains the following methods:

* **find**: returns the document for the provided path
* **findMany**: returns an array of documents for the provided paths
* **isPublished**: checks if the provided document is published
* **getPrev**: returns the previous document by examining the child nodes of
  the provided parent
* **getPrevLinkable**: returns the previous linkable document by examining
  the child nodes of the provided parent
* **getNext**: returns the next document by examining the child nodes of the
  provided parent
* **getNextLinkable**: returns the next linkable document by examining the
  child nodes of the provided parent
* **getChild**: returns a child documents of the provided parent document and
  child node
* **getChildren**: returns an array of all the children documents of the
  provided parent
* **getLinkableChildren**: returns an array of all the linkable children
  documents of the provided parent
* **getDescendants**: returns an array of all descendants paths of the
  provided parent
* **getLocalesFor**: gets the locales of the provided document
* **getNodeName**: returns the node name of the provided document
* **getParentPath**: returns the parent path of the provided document
* **getPath**: returns the path of the provided document

.. code-block:: php

    <?php $page = $view['cmf']->find('/some/path') ?>

    <?php if $view['cmf']->isPublished($page) : ?>
        <?php $prev = $view['cmf']->getPrev($page) ?>
        <?php if ($prev) : ?>
            <a href="<?php echo $view['router']->generate($prev) ?>">prev</a>
        <?php endif ?>

        <?php $next = $view['cmf']->getNext($page) ?>
        <?php if ($next) : ?>
            <span style="float: right; padding-right: 40px;">
                <a href="<?php echo $view['router']->generate($next) ?>">next</a>
            </span>
        <?php endif ?>

        <?php foreach (array_reverse($view['cmf']->getChildren($page)) as $news) : ?>
            <li>
                <a href="<?php echo $view['router']->generate($news) ?>"><?php echo $news->getTitle() ?></a>
                (<?php echo date('Y-m-d', $news->getPublishStartDate()) ?>)
            </li>
        <?php endforeach ?>

        <?php if (in_array('de', $view['cmf']->getLocalesFor($page))) : ?>
            <a href="<?php $view['router']->generate
                $app->getRequest()->attributes->get('_route'),
                array_merge(
                    $app->getRequest()->attributes->get('_route_params'),
                    array_merge(
                        $app->getRequest()->query->all(),
                        array('_locale' => 'de')
                    )
                )
            ?>">DE</a>
        <?php endif ?>
        <?php if (in_array('fr', $view['cmf']->getLocalesFor($page))) : ?>
            <a href="<?php $view['router']->generate
                $app->getRequest()->attributes->get('_route'),
                array_merge(
                    $app->getRequest()->attributes->get('_route_params'),
                    array_merge(
                        $app->getRequest()->query->all(),
                        array('_locale' => 'fr')
                    )
                )
            ?>">FR</a>
        <?php endif ?>
    <?php endif ?>

.. _`CoreBundle`: https://github.com/symfony-cmf/CoreBundle#readme
.. _`Symfony2 security component`: http://www.symfony.com/doc/current/components/security/index.html
.. _`Symfony2 Authorization`: http://www.symfony.com/doc/current/components/security/authorization.html
.. _`Security Chapter`: http://www.symfony.com/doc/current/book/security.html
.. _`ACL checks`: http://www.symfony.com/doc/current/cookbook/security/acl.html
.. _`Sonata Admin extension documentation`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _`synchronized service`: http://symfony.com/doc/current/cookbook/service_container/scopes.html#using-a-synchronized-service
