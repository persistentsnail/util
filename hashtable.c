class ObjHashTable
{
		public:
				struct Node
				{
						int _key;
						Node *_next;
				};
		protected:
				Node **_buckets;
				int _nbucket;
				enum {INIT_BUCKET_SIZE = 10, INIT_NODES_POOL = 3000};

				Node *_nodes_pool;
				unsigned _nodes_used;
				unsigned _pool_size;

		public:
				ObjHashTable()
				{
						_buckets = (Node **)malloc(sizeof(Node *) * (1 << INIT_BUCKET_SIZE));
						_nbucket = 1 << INIT_BUCKET_SIZE;
						memset(_buckets, NULL, sizeof(Node *) * _nbucket);

						_nodes_pool = (Node *)malloc(sizeof(Node) * INIT_NODES_POOL);
						_pool_size = INIT_NODES_POOL;
						_nodes_used = 0;
				}
				~ObjHashTable()
				{
						free(_nodes_pool);
						free(_buckets);
				}
				void Insert(int obj)
				{
						if (_nodes_used >= _pool_size)
						{
								_pool_size <<= 1;
								_nodes_pool = (Node *)realloc(_nodes_pool, _pool_size);
								// rehash
								memset(_buckets, NULL, sizeof(Node *) * _nbucket);
								for (unsigned i = 0 ; i < _nodes_used; i++)
								{
										int pos_bucket = _nodes_pool[i]._key & (_nbucket - 1);
										Node *&pos_p = _buckets[pos_bucket];
										_nodes_pool[i]._next = pos_p;
										pos_p = &_nodes_pool[i];
								}
						}
						int bucket_id = obj & (_nbucket - 1); 
						Node *&obj_p = _buckets[bucket_id];

						Node *obj_n = &_nodes_pool[_nodes_used++];
						obj_n->_next = obj_p;
						obj_n->_key = obj;
						obj_p = obj_n;
				}
				void Delete(int obj)
				{
						int bucket_id = obj & (_nbucket - 1);
						Node **pp = &_buckets[bucket_id];
						Node *recycle = NULL;
						while (*pp)
						{
								if ((*pp)->_key == obj)
								{
										recycle = *pp;
										*pp = recycle->_next;
										break;
								}
								pp = &(*pp)->_next;
						}
						if (!recycle)
						{
								assert(false);
								return ;
						}
						/* do recycle */
						assert(_nodes_used >= 1);
						Node *N = &_nodes_pool[--_nodes_used];
						if (N == recycle)
								return;
						pp = &_buckets[N->_key & (_nbucket - 1)];
						while (*pp)
						{
								if (*pp == N)
								{
										*recycle = *N;
										*pp = recycle;
										break;
								}
								pp = &(*pp)->_next;
						}
				}

				int GetAllNodes(Node **nodes)
				{
						*nodes = _nodes_pool;
						return _nodes_used;
				}
};
