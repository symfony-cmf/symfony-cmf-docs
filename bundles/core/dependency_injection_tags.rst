.. index::
    single: Dependency Injection Tags; CoreBundle

Dependency Injection Tags
-------------------------

.. _bundle-core-tags-request-aware:

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

.. caution::

    You should only use this tag on services that will be needed on every
    request. If you use this tag excessively you will run into performance
    issues. For seldom used services, you can inject the container in the
    service definition and call ``$this->container->get('request')`` in your
    code when you actually need the request.

For Symfony 2.3, this tag is automatically translated to a
`synchronized service`_ but Symfony 2.2 does not have that feature, so you can
use this tag for bundles that you want to be able to work with Symfony 2.2. In
custom applications that run with Symfony 2.3, there is no need for this tag,
just use the synchronized service feature.

cmf_published_voter
~~~~~~~~~~~~~~~~~~~

Used to activate :ref:`custom voters <bundle-core-workflow-custom-voters>` for the
:doc:`publish workflow <publish_workflow>`. Tagging a service with
``cmf_published_voter`` integrates it into the access decision of the publish
workflow.

This tag has the attribute ``priority``. The lower the priority number, the
earlier the voter gets to vote.

.. _`synchronized service`: http://symfony.com/doc/current/cookbook/service_container/scopes.html#using-a-synchronized-service
