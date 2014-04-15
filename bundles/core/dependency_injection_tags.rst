.. index::
    single: Dependency Injection Tags; CoreBundle

Dependency Injection Tags
-------------------------

cmf_request_aware
~~~~~~~~~~~~~~~~~

.. caution::

    This tag has been deprecated in CoreBundle 1.1 and will be removed
    in CoreBundle 1.2. Since Symfony 2.3, you can profit from the fact
    that the request is a `synchronized service`_.

When working with the 1.0 version of the CMF in Symfony 2.2 and you have
services that need the request (e.g. for the publishing workflow or current
menu item voters), you can tag services with ``cmf_request_aware`` to have a
kernel listener inject the request. Any class used in such a tagged service
must have the ``setRequest`` method or you will get a fatal error::

    use Symfony\Component\HttpFoundation\Request;

    class MyClass
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }
    }

cmf_published_voter
~~~~~~~~~~~~~~~~~~~

Used to activate :ref:`custom voters <bundle-core-workflow-custom-voters>` for the
:doc:`publish workflow <publish_workflow>`. Tagging a service with
``cmf_published_voter`` integrates it into the access decision of the publish
workflow.

This tag has the attribute ``priority``. The lower the priority number, the
earlier the voter gets to vote.

.. _`synchronized service`: http://symfony.com/doc/current/cookbook/service_container/scopes.html#a-using-a-synchronized-service
