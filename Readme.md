# A demo game server

## 关系图

```mermaid
graph TD
es[epoll_server] --> ctrl[controller]
ctrl --> re[region]
ctrl --> pl[player]
```