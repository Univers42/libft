# ft_queue_enqueue

Adds an element to the rear of the queue.

## Synopsis

```c
bool ft_queue_enqueue(t_queue *queue, void *data);
```

## Description

Appends `data` at the queue's rear in O(1). The queue does not own the memory pointed by `data`.

## Return Value

`true` on success, `false` on failure (e.g., allocation error).
